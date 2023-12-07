#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void set_bit(char bit, char *byte);
void clear_bit(char bit, char *byte);
int get_bit(char bit, char byte);
void init_heap(char heap[]);
void set_block_sz(char *byte, int size);
int get_block_sz(char byte);
void print_byte(char byte);
double raise(int base, int exp);
int block_allocated(char byte);
int first_fit(char heap[], int payload);
int best_fit(char heap[], int free_blocks_asc[], int payload);
int free_block(char heap[], int payload);
int noaddressinblocksdesc(int blocks_desc[], int size, int address);
void fill_blocks_desc(char heap[], int blocks_desc[]);
void fill_free_blocks_asc(char heap[], int free_blocks_asc[]);

// globals
int totalblocks = 1;

int main(int argc, char *argv[]) {
    if (argv[1] != NULL && strcmp(argv[1], "FirstFit") != 0 && strcmp(argv[1], "BestFit") != 0) {
        printf("must enter FirstFit, BestFit, or nothing for FirstFit default\n");
        return 1;  
    }

    char heap[127] = {0};
    char *token = NULL, *tokptr = NULL;
    char cmdbuf[256] = {0};
    int blocks_desc[42] = {[0 ... 41] = -1}; // 42 x 3 byte min = 126
    int free_blocks_asc[42] = {[0 ... 41] = -1}; // 42 x 3 byte min = 126

    init_heap(heap);

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
            return 0;
        }
        // malloc
        if (strcmp(token, "malloc") == 0) {
            token = strtok_r(NULL, " \n", &tokptr);

			if (token == NULL) {
				printf("malloc must have a payload size\n");
				continue;
			}

            int payloadaddr;

            if (argv[1] == NULL || strcmp(argv[1], "FirstFit") == 0) {
                payloadaddr = first_fit(heap, atoi(token)); 
            }
            else if (strcmp(argv[1], "BestFit") == 0) {
                payloadaddr = best_fit(heap, free_blocks_asc, atoi(token)); 
            }
            
            if (payloadaddr == 0) {
                printf("malloc request too large\n");
                continue;
            }                
            else if (payloadaddr == -1) {
                printf("invalid malloc request size\n");
                continue;
            }
            else {
                printf("%d\n", payloadaddr);
            }
        }
        // free 
        else if (strcmp(token, "free") == 0) {
            token = strtok_r(NULL, " \n", &tokptr);
			if (token == NULL) {
				printf("free must have an address to free\n");
				continue;
            }

            int result = free_block(heap, atoi(token));

            if (result == -1) {
                printf("the address in not valid\n");
                continue;
            }
            if (result == 0) {
                printf("the address in not freeable\n");
                continue;
            }
        }
        // blocklist
        else if (strcmp(token, "blocklist") == 0) {
            fill_blocks_desc(heap, blocks_desc);

            for (int i = 0; i < totalblocks; i++) {
                printf("%d-%d-%s\n",
                get_block_sz(heap[blocks_desc[i]]) - 2, // minus 2 for payload only
                blocks_desc[i] + 1,
                block_allocated(heap[blocks_desc[i]]) ? "allocated" : "free");
            }            
        } 
        // writemem 
        else if (strcmp(token, "writemem") == 0) {
            token = strtok_r(NULL, " \n", &tokptr);
			if (token == NULL) {
				printf("writemem must have an address\n");
				continue;
			}
            int address = atoi(token);           
            char str[125] = {0};

            token = strtok_r(NULL, " \n", &tokptr);
			if (token == NULL) {
				printf("writemem must have a string argument\n");
				continue;
			}
            strcpy(str, token);

            int i = address;
            for (int j = 0; str[j] != '\0'; i++, j++) {
                heap[i] = str[j];
            } 
        }
        // printmem 
        else if (strcmp(token, "printmem") == 0) {
            token = strtok_r(NULL, " \n", &tokptr);
            if (token == NULL) {
                printf("printmem must have an address\n");
                continue;
            }
            int address = atoi(token);           
            char str[125] = {0};

            token = strtok_r(NULL, " \n", &tokptr);
            if (token == NULL) {
                printf("writemem must have number of characters to print\n");
                continue;
            }
            int numofchars = atoi(token);

            for (int i = address; i < address + numofchars; i++) {
                printf("%d", heap[i]);
                if ((i + 1) < (address + numofchars)) {
                    printf("-");
                }
            }
            printf("\n");
        }
        // pb
        else if (strcmp(token, "pb") == 0) {
            token = strtok_r(NULL, " \n", &tokptr);
            print_byte(heap[atoi(token)]);
            printf("\n%d byte(s)\n", get_block_sz(heap[atoi(token)]));
        }
        // pba
        else if (strcmp(token, "pba") == 0) {
            for (int i = 0; i < 127; i++) {
                printf("[%d] ", i);
                print_byte(heap[i]);
                printf(" %d byte(s)\n", get_block_sz(heap[i]));
            }
        }
        else {
            printf("invalid command\n");
        }
    }
    return 0;
}

void set_bit(char bit, char *byte) {
    *byte = *byte | (1 << bit);
}

void clear_bit(char bit, char *byte) {
    *byte = *byte & ~(1 << bit);
}

int get_bit(char bit, char byte) {
    return ((byte & (1 << bit)) != 0);
}

void set_block_sz(char *byte, int size) {
    *byte = size << 1;
}
// return the sum of the 7 most significant bits
int get_block_sz(char byte) {
    int sum = 0;
    for (int i = 0; i < 7; i++) {
        if (byte & (1 << i + 1)) {
            sum += (int)raise(2, i);
        }
    }
    return sum;
}

void init_heap(char heap[]) {
    set_block_sz(&heap[0], 127);
    set_block_sz(&heap[126], 127);
}

void print_byte(char byte) {
    for (int i = 7; 0 <= i; i--) {
        printf("%c", (byte & (1 << i)) ? '1' : '0');
    }
}

double raise(int base, int exp) {
    double result = 1.0;
    while (exp != 0) {
        result *= base;
        exp--;
    }
    return result;
}

int block_allocated(char byte) {
    return get_bit(0, byte);
}

int first_fit(char heap[], int payload) {
    if (payload < 1 || payload > 125) {
        return -1;
    }

    int address = 0, split = 1;
    int currentsize, newsize, sizediff;

    while (address <= 124) { // allowing for 3 bytes in last block
        currentsize = get_block_sz(heap[address]);
        newsize = payload + 2;

        if (!block_allocated(heap[address]) && currentsize >= newsize) {
            sizediff = currentsize - newsize;

            if (sizediff < 3) {
                newsize += sizediff;
                split = 0;
            }
            // set size of new block (to include header and footer)
            set_block_sz(&heap[address], newsize); 
            // set allocated bit to 1
            set_bit(0, &heap[address]);
            // copy this header to correct place in heap for footer
            heap[address + newsize - 1] = heap[address];

            if (split) {
                // no need to set alloc bit to 0 since it is already
                // set new header (address after footer) to new split-block size
                set_block_sz(&heap[address + newsize], sizediff);
                // copy this header to correct place in heap for footer
                heap[address + newsize + sizediff - 1] = heap[address + newsize];
                totalblocks++;
            }
            return address + 1;
        }
        else {
            address += get_block_sz(heap[address]);
        }
    }
    return 0;
}

int best_fit(char heap[], int free_blocks_asc[], int payload) {
    if (payload < 1 || payload > 125) {
        return -1;
    }

    int address = 0, split = 1;
    int currentsize, newsize, sizediff;

    fill_free_blocks_asc(heap, free_blocks_asc);

    for (int i = 0; free_blocks_asc[i] != -1; i++) {
        address = free_blocks_asc[i];
        currentsize = get_block_sz(heap[address]);
        newsize = payload + 2;

        if (newsize <= currentsize) {
            sizediff = currentsize - newsize;

            if (sizediff < 3) {
                newsize += sizediff;
                split = 0;
            }
            // set size of new block (to include header and footer)
            set_block_sz(&heap[address], newsize); 
            // set allocated bit to 1
            set_bit(0, &heap[address]);
            // copy this header to correct place in heap for footer
            heap[address + newsize - 1] = heap[address];

            if (split) {
                // no need to set alloc bit to 0 since it is already
                // set new header (address after footer) to new split-block size
                set_block_sz(&heap[address + newsize], sizediff);
                // copy this header to correct place in heap for footer
                heap[address + newsize + sizediff - 1] = heap[address + newsize];
                totalblocks++;
            }
            return address + 1;
        }
        else {
            address += get_block_sz(heap[address]);
        }
    }
    return 0;
}

int free_block(char heap[], int payload) {
    if (payload < 1 || payload > 125) {
        return -1;
    }
    int header = payload - 1;
    int blocksize = get_block_sz(heap[header]);
    int footer = header + blocksize - 1;
    int prevblocksize, nextblocksize;

    if (block_allocated(heap[header])) {
        // set payload elements to 0
        for (int i = payload; i <= (footer - 1); i++) {
            heap[i] = 0;
        }
        // set head and footer to free
        clear_bit(0, &heap[header]);
        clear_bit(0, &heap[footer]);
    
        // if footer of previous block is free
        if ((header - 1 > 0) && !block_allocated(heap[header - 1])) {
            prevblocksize = get_block_sz(heap[header - 1]);
            // set previous header's block-size to sum of previous + current
            set_block_sz(&heap[header - prevblocksize], prevblocksize + blocksize);
            // copy header of previous to current footer
            heap[footer] = heap[header - prevblocksize];
            // set previous footer and current header to 0
            heap[header - 1] = 0;
            heap[header] = 0;
            header = header - prevblocksize;
            blocksize = get_block_sz(heap[header]);
            totalblocks--;
        }
        // if header of next block is free
        if ((footer + 1 < 127) && !block_allocated(heap[footer + 1])) {
            nextblocksize = get_block_sz(heap[footer + 1]);
            // set current header's block-size to sum of current + next
            set_block_sz(&heap[header], blocksize + nextblocksize);
            // copy current header to footer of next
            heap[footer + nextblocksize] = heap[header];
            // set next header and current footer to 0
            heap[footer + 1] = 0;
            heap[footer] = 0;
            totalblocks--;
        }
        return 1;
    }
    return 0;
}

int noaddressinblocksdesc(int blocks_desc[], int size, int address) {
    for (int i = 0; i < size; i++) {
        if (blocks_desc[i] == address) {
            return 0;
        }
    }
    return 1;
}

void fill_blocks_desc(char heap[], int blocks_desc[]) {
    int address, maxblocksize, maxblockaddr, currentblocksize;
    for (int i = 0; i < totalblocks; i++) {
        address = 0;
        maxblocksize = 0;
        while (address <= 124) {
            currentblocksize = get_block_sz(heap[address]);  

            if (currentblocksize > maxblocksize && noaddressinblocksdesc(blocks_desc, i, address)) {
                maxblocksize = currentblocksize;
                maxblockaddr = address;
            } 

            address += get_block_sz(heap[address]);
        }
        blocks_desc[i] = maxblockaddr;
    }
}

int noaddressinfreeblocksasc(int free_blocks_asc[], int size, int address) {
    for (int i = 0; i < size; i++) {
        if (free_blocks_asc[i] == address) {
            return 0;
        }
    }
    return 1;
}

void fill_free_blocks_asc(char heap[], int free_blocks_asc[]) {
    int address, minblocksize, minblockaddr, currentblocksize;
    
    for (int i = 0; i < totalblocks; i++) {
        address = 0;
        minblocksize = 128;
        minblockaddr = -1;
        while (address <= 124) {
            if (!block_allocated(heap[address])) {
                currentblocksize = get_block_sz(heap[address]);  

                if (currentblocksize < minblocksize && noaddressinfreeblocksasc(free_blocks_asc, i, address)) {
                    minblocksize = currentblocksize;
                    minblockaddr = address;
                } 
            }
            address += get_block_sz(heap[address]);
        }
        // if no blocks are free in this iteration of i, then minblockaddr = -1
        free_blocks_asc[i] = minblockaddr;
    }
    free_blocks_asc[totalblocks] = -1;
}