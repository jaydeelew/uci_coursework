// add events to a calendar in the command format:
// add eventName startTime(int) Endtime(int)
// delete events: delete eventName
// print calendar: printCal
// quit program: quit
// calendar start and end times must always be in ascending order
// events must not overlap

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// linked list node
typedef struct event {
    char *name;
    int start;
    int end;
    struct event *next;
} event_t;

// linked list
typedef struct dayList {
    event_t *head;
} dayList_t;

// function declarations (a.k.a function prototypes)
int isNumber(char *str);
int check_hour(int hour);
int addEvent(dayList_t *list, char *name, int start, int end); 
void deleteEvent(dayList_t *list, char *name);
void printDayList(dayList_t *list);
void deleteAll(dayList_t *list);


int main() {
    char str[255] = {0};
    char *token = NULL, *tokptr = NULL, *name = NULL;
    int start = 0, end = 0, len = 0;
    dayList_t myDay = {NULL}; // assigns NULL to myDay->head
    dayList_t *myDayPtr = &myDay; // address of myDay object assigned to myDay pointer

    while (1) {
        printf("$ ");
        fgets(str, 255, stdin);
        token = strtok_r(str, " \n", &tokptr);
    
        if (strcmp(token, "add") == 0) {
            token = strtok_r(NULL, " ", &tokptr);
            if (token == NULL || *token == '\n') {
                fprintf(stderr, "Name not provided error\n");
                continue; 
            }

            len = strlen(token);
            name = malloc(len + 1);

            while(*token) {
                *name = *token;
                token++;
                name++;
            }
            *name = '\0';
            name = name - len;

            token = strtok_r(NULL, " \n", &tokptr);
            if (token == NULL || *token == '\n') {
                fprintf(stderr, "Hour not provided error\n");
                free(name);
                continue; 
            }
            if (isNumber(token))
                start = atoi(token);
            else {
                fprintf(stderr, "Start is not a number error\n");
                free(name);
                continue;
            }

            token = strtok_r(NULL, " \n", &tokptr);
            if (token == NULL || *token == '\n') {
                fprintf(stderr, "Hour not provided error\n");
                free(name);
                continue; 
            }
            if (isNumber(token))
                end = atoi(token);
            else {
                fprintf(stderr, "End is not a number error\n");
                free(name);
                continue;
            }

            addEvent(myDayPtr, name, start, end);
        }
        else if (strcmp(token, "delete") == 0) {
            token = strtok_r(NULL, " \n", &tokptr);
            if (token == NULL || *token == '\n') {
                fprintf(stderr, "Name not provided error\n");
                continue; 
            }
            deleteEvent(myDayPtr, token);
        }
        else if (strcmp(token, "printCal") == 0) {
            printDayList(myDayPtr);
        }
        else if (strcmp(token, "quit") == 0) {
            deleteAll(myDayPtr);
            return 0;
        }
        else {
            printf("Invalid command entered\n");
        }
    }

    return 0;
}

int isNumber(char *str) {
    while (*str != '\0') {   
        if (isdigit(*str) == 0)
              return 0;
        str++;
    }

    return 1;
}


int check_hour(int hour) { 
    if (hour >= 0 && hour <= 23) 
        return hour;
    else
        return -1;
}

int addEvent(dayList_t *list, char *name, int start, int end) {
    if (end <= start) {
        fprintf(stderr, "Invald hours error\n");
        free(name);
    }
    else if (check_hour(start) == -1 || check_hour(end) == -1) {
        fprintf(stderr, "Invalid hours error\n");
        free(name);
    }
    // if there are no events... INSERT HEAD EVENT  
    else if (list->head == NULL) {
        event_t *temp = malloc(sizeof(event_t));
        temp->name = name;
        temp->start = start;
        temp->end = end;
        temp->next = NULL;

        list->head = temp;
    }
    // if end of new event is <= start of head event... INSERT BEFORE HEAD
    else if (end <= list->head->start) {
        event_t *temp = malloc(sizeof(event_t));
        temp->name = name;
        temp->start = start;
        temp->end = end;
        temp->next = list->head; // head becomes next event

        list->head = temp; // new event becomes head
    }
    else {
        event_t *current = list->head; // make head the current node
        while (1) {
            /* if start of new event is >= end of current event
             * AND (end of new event is <= start of next event
             * OR next event is NULL)
             * then INSERT AFTER current
             */ 
            if (start >= current->end && (current->next == NULL || end <= current->next->start)) {
                event_t *temp = malloc(sizeof(event_t));
                temp->name = name;
                temp->start = start;
                temp->end = end;
                // new event's next-node becomes current node's next-node
                temp->next = current->next;

                // current event's next-node becomes the new event
                current->next = temp;
                break;
            }
            else {
                current = current->next;
                if (current == NULL) {
                    fprintf(stderr, "Event overlap error\n");
                    free(name);
                    break;
                }
            }
        }
    }
    return 1;
}

void deleteEvent(dayList_t *list, char *name) {
    event_t *currentEvent = list->head;
    event_t *previousEvent = NULL;
    if (currentEvent == NULL)
        printf("The day does not have any events scheduled\n");
    else if (strcmp(currentEvent->name, name) == 0) {
                // then delete head event
                list->head = currentEvent->next;
                free(currentEvent->name);
                free(currentEvent);
    }
    else {
        previousEvent = currentEvent;
        currentEvent = currentEvent->next;

        while (currentEvent != NULL) {
            if (strcmp(currentEvent->name, name) == 0) {
                previousEvent->next = currentEvent->next;
                free(currentEvent->name);
                free(currentEvent);
                return;
            }  
            previousEvent = currentEvent;
            currentEvent = currentEvent->next;
        }

        printf("%s is not an event\n", name);
    }
}

void printDayList(dayList_t *list) {
    event_t *currentEvent = list->head;
    if (currentEvent == NULL)
        printf("The day does not have any events scheduled\n");
    else
        do {
            printf("%s %d %d\n", currentEvent->name, currentEvent->start, currentEvent->end);
            currentEvent = currentEvent->next;
        } while (currentEvent != NULL);
}

void deleteAll(dayList_t *list) {
    if (list != NULL) {
        event_t *currentEvent = list->head;
        event_t *temp;
        while (currentEvent != NULL) {
            temp = currentEvent->next;
            free(currentEvent->name);
            free(currentEvent);
            currentEvent = temp;
        }
    }
}