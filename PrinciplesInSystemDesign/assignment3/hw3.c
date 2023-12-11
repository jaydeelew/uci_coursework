#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* 
virtual memory (vm) has 128 virtual addresses (va)
physical memory (pm) has 32 physical addresses (pa)
the page/frame size is 8 (each contain 8 addresses)
vm is comprised of 128/8 = 16 pages (virtual pages)
pm is comprised of 32/8 = 4 frames (physical pages)
*/
typedef struct page {
    int pageNum; // virtual page
    int valid;
    int dirty;
    int frameNum; // physical page
} page_t;

typedef struct node {
    int val;
    struct node *next;
} node_t;

typedef struct queue {
    int len;
    node_t *head;
    node_t *tail;
} queue_t;

void initPageTable(page_t pt[], int size);
void getPageAndOffset(int va, int *page, int *offset);
int isNumber(char *str);
void initQueue(queue_t *q, int size);
void enqueue(queue_t *q, int frame);
int dequeue(queue_t *q);
void printQueue(queue_t *q);
int lookupPageWithFrame(page_t pt[], int frame);
void showmain(int pm[], int frame);
void showptable(page_t pt[]);
void freeQueue(queue_t *q); 
int lruVictim(void);
void printLRU(void);

// Globals
int counter = 0;
int lrucounts[4] = {0};
int pg_sz = 8;
int pg_tbl_sz = 16;
int q_sz = 4;

int main(int argc, char *argv[]) {
    if (argv[1] != NULL && strcmp(argv[1], "FIFO") != 0 && strcmp(argv[1], "LRU") != 0) {
        printf("must enter FIFO, LRU, or nothing for FIFO default\n");
        return 1;  
    }
    char cmdbuf[256] = {0};
    int va; // virtual address
    int num; // value to hold at address
    int vm[128] = {[0 ... 127] = -1}; // virtual memory
    int pm[32] = {[0 ... 31] = -1}; // physical memory
    int page; // virtual page
    int frame; // physical page
    int offset;
    int victimpage;
    int victimframe;
    int lookuppage;
    page_t pagetable[pg_tbl_sz];
    char *token = NULL, *tokptr = NULL;
    queue_t FIFO_q;

    initPageTable(pagetable, pg_tbl_sz);
    initQueue(&FIFO_q, q_sz);

    while (1) {
        printf("> ");
        fgets(cmdbuf, 256, stdin);
		token = strtok_r(cmdbuf, " \n", &tokptr);
		// NULL
		if (token == NULL) {
			printf("a command was not entered\n");
			continue;
		}
        // quit
		if (strcmp(token, "quit") == 0) {
            freeQueue(&FIFO_q);
            return 0;
        }
        // read 
		if (strcmp(token, "read") == 0) {
            token = strtok_r(NULL, " \n", &tokptr);

			if (token == NULL) {
				printf("a virtual address must be entered\n");
				continue;
			}
            if (atoi(token) < 0 || atoi(token) > 127 || !isNumber(token)) {
                printf("address must be a number from 0 to 127\n");
                continue;
            }
            else { // REMOVE ELSE
                va = atoi(token);
                getPageAndOffset(va, &page, &offset);
                // query pagetable with page to see if valid bit is set to 1
                if (pagetable[page].valid == 1) {
                    // if so, query pm with frame from pagetable and offset to retrieve value from frame 
                    printf("%d\n", pm[(pagetable[page].frameNum * pg_sz) + offset]); 
                    lrucounts[pagetable[page].frameNum] = ++counter;
                }
                else {
                    printf("a page fault has occurred\n");
                    // select victim from pm based on argv[1]
                    if (argv[1] == NULL || strcmp(argv[1], "FIFO") == 0) {
                        victimframe = dequeue(&FIFO_q);          
                    }
                    else if (strcmp(argv[1], "LRU") == 0) {
                        victimframe = lruVictim();
                    }
                    
                    lookuppage = lookupPageWithFrame(pagetable, victimframe); 
                    // if there is a pte for this victimframe
                    if (lookuppage != -1) {
                        // the page table is updated to show the evicted page is back on disk
                        pagetable[lookuppage].valid = 0;
                        pagetable[lookuppage].frameNum = lookuppage;
                        // if dirty bit of victim is 1, copy page from pm to vm and set dirty and valid to zero 
                        if (pagetable[lookuppage].dirty == 1) {
                            for (int i = 0; i < pg_sz; i++) {
                                vm[(lookuppage * pg_sz) + i] = pm[(victimframe * pg_sz) + i];
                            }
                            pagetable[lookuppage].dirty = 0;
                        }
                    }
                    // copy desired page from vm to where victim is in pm
                    for (int i = 0; i < pg_sz; i++) {
                        pm[(victimframe * pg_sz) + i] = vm[(page * pg_sz) + i];
                    }
                    // record new frame address in pagetable
                    pagetable[page].frameNum = victimframe;
                    // mark pte as valid
                    pagetable[page].valid = 1;
                    // place new frame number in queue
                    enqueue(&FIFO_q, victimframe);
                    // query pm with frame from pagetable and offset to retrieve value from frame 
                    printf("%d\n", pm[(pagetable[page].frameNum * pg_sz) + offset]); 
                    lrucounts[pagetable[page].frameNum] = ++counter;
                }
            }
        }
        // write
		else if (strcmp(token, "write") == 0) {
            token = strtok_r(NULL, " \n", &tokptr);

			if (token == NULL) {
				printf("a virtual address must be entered\n");
				continue;
			}
            if (atoi(token) < 0 || atoi(token) > 127 || !isNumber(token)) {
                printf("address must be a number from 0 to 127\n");
                continue;
            }
            else { // REMOVE ELSE
                va = atoi(token);
                token = strtok_r(NULL, " \n", &tokptr);

                if (token == NULL) {
                    printf("a number must be entered\n");
                    continue;
                }
                if (!isNumber(token)) {
                    printf("an integer value must be entered after the address\n");
                    continue;
                }
                else {
                    num = atoi(token);
                    getPageAndOffset(va, &page, &offset);
                    // query pagetable with page to see if valid bit is set to 1
                    if (pagetable[page].valid == 1) {
                        // if so, query pm with frame from pagetable and offset to set the value in the frame 
                        pm[(pagetable[page].frameNum * pg_sz) + offset] = num; 
                        // set dirty bit to 1 for this pte
                        pagetable[page].dirty = 1;
                        lrucounts[pagetable[page].frameNum] = ++counter;
                    }
                    else {
                        printf("a page fault has occurred\n");
                        // select victim from pm based on argv[1]
                        if (argv[1] == NULL || strcmp(argv[1], "FIFO") == 0) {
                            victimframe = dequeue(&FIFO_q);          
                        }
                        else if (strcmp(argv[1], "LRU") == 0) {
                            victimframe = lruVictim();
                        }
                        
                        lookuppage = lookupPageWithFrame(pagetable, victimframe); 
                        // if there is a pte for this victimframe
                        if (lookuppage != -1) {
                            // the page table is updated to show the evicted page is back on disk
                            pagetable[lookuppage].valid = 0;
                            pagetable[lookuppage].frameNum = lookuppage;
                            // if dirty bit of victim is 1, copy page from pm to vm and set dirty and valid to zero 
                            if (pagetable[lookuppage].dirty == 1) {
                                for (int i = 0; i < pg_sz; i++) {
                                    vm[(lookuppage * pg_sz) + i] = pm[(victimframe * pg_sz) + i];
                                }
                                pagetable[lookuppage].dirty = 0;
                            }
                        }
                        // copy desired page from vm to where victim is in pm
                        for (int i = 0; i < pg_sz; i++) {
                            pm[(victimframe * pg_sz) + i] = vm[(page * pg_sz) + i];
                        }
                        // record new frame address in pagetable
                        pagetable[page].frameNum = victimframe;
                        // mark pte as valid
                        pagetable[page].valid = 1;
                        // place new frame number in queue
                        enqueue(&FIFO_q, victimframe);
                        // query pm with frame from pagetable plus offset to set the value at the address in the frame 
                        pm[(pagetable[page].frameNum * pg_sz) + offset] = num; 
                        // set dirty bit to 1 for this pte
                        pagetable[page].dirty = 1;
                        lrucounts[pagetable[page].frameNum] = ++counter;
                    }
                }
            }
        }
        // showmain
		else if (strcmp(token, "showmain") == 0) {
            token = strtok_r(NULL, " \n", &tokptr);

			if (token == NULL) {
				printf("a frame number must be entered\n");
				continue;
			}
			if (atoi(token) < 0 || atoi(token) > 3) {
				printf("frame number must be between 0 and 3\n");
				continue;
			}

            frame = atoi(token);
            showmain(pm, frame); 
        }
        // showptable
		else if (strcmp(token, "showptable") == 0) {
            showptable(pagetable);
        }
        // pq REMOVE AFTER TESTING
		else if (strcmp(token, "pq") == 0) { 
            printQueue(&FIFO_q);
        }
        // pl REMOVE AFTER TESTING
		else if (strcmp(token, "pl") == 0) { 
            printLRU();            
        }
        else {
            printf("invalid command\n");
        }
    }

    return 0;
}

void initPageTable(page_t pt[], int size) {
    for (int i = 0; i < size; i++) {
        pt[i].pageNum = i;
        pt[i].valid = 0;
        pt[i].dirty = 0;
        pt[i].frameNum = i;
    }
}

void getPageAndOffset(int va, int *page, int *offset) {
    *page = va / pg_sz; // 17 / 8 = 2 (.125 ommitted because integer math)
    float fraction = (float)(va % pg_sz) / 8; // (17 % 8) / 8 = .125
    *offset = (int)(pg_sz * fraction); // 8 x .125 = 1
}

int isNumber(char *str) {
    while (*str != '\0') {   
        if (isdigit(*str) == 0)
              return 0;
        str++;
    }
    return 1;
}

void initQueue(queue_t *q, int size) {
    q->len = size;
    q->head = NULL;
    q->tail = NULL;
    for (int i = 0; i < size; i++) {
        enqueue(q, i);
    }
}
// adds to end of queue
void enqueue(queue_t *q, int frame) {
    if (q->head == NULL) {
        node_t *new = malloc(sizeof(node_t));
        new->val = frame;
        new->next = NULL;
        q->head = new;
        q->tail = new;
        q->len = 1;
    }
    else {
        node_t *new = malloc(sizeof(node_t));
        new->val = frame;
        new->next = NULL;
        q->tail->next = new;
        q->tail = q->tail->next;
        q->len++;
    }
}
// removes from front of queue
int dequeue(queue_t *q) {
    int val = q->head->val;
    node_t *temp = q->head;
    q->head = q->head->next;
    free(temp);
    q->len--;
    return val;
}
// looks up page number of correspondnig frame number for entries currently in pm
int lookupPageWithFrame(page_t pt[], int frame) {
    for (int i = 0; i < pg_tbl_sz; i++) {
        if (pt[i].frameNum == frame && pt[i].valid == 1) {
            return pt[i].pageNum;
        }
    }
    return -1;
}

void showmain(int pm[], int frame) {
    int i, j = 0; 
    for (i = frame * pg_sz; j < pg_sz; i++, j++) {
        printf("%d: %d\n", i, pm[i]);
    }
}

void showptable(page_t pt[]) {
    for (int i = 0; i < pg_tbl_sz; i++) {
        printf("%d:%d:%d:%d\n", pt[i].pageNum, pt[i].valid, pt[i].dirty, pt[i].frameNum);
    }
}

void freeQueue(queue_t *q) {
    node_t *current = q->head;
    node_t *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
}

void printQueue(queue_t *q) {
    node_t *current = q->head;
    while (current != NULL) {
        printf("%d ", current->val);
        current = current->next;
    }
    printf("\n");
}
// return index/frame of least of lrucounts elements
int lruVictim(void) {
    int least = 500, frame;
    if (counter < 4) {
        return counter; 
    } 
    else {
        for (int i = 0; i < 4; i++) {
            if (lrucounts[i] < least) {
                least = lrucounts[i];
                frame = i;
            }
        }
        return frame;
    }
}

void printLRU(void) {
    for (int i = 0; i < 4; i++) {
        printf("%d ", lrucounts[i]);
    }
    printf("\n");
}