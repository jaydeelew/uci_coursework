// Jack Lewis 
// 26197631

#include "hw2.h"
#include "helpers2.h"
#include "linkedlist.h"

// Part 1 Functions to implement                     MAKE USE OF USAGE STATEMENT UPON ERROR
int getDate(char* str, Date* myDate) {

    if (str == NULL || myDate == NULL) {
        //usage function
        return 0;
    }
    
    char *s;
    int m, d, y, idx = 0;
    for (s = str; *s; s++) {
        if ((*s < '0' || *s > '9') && *s != '/') {
            //usage function
            printf("Line 18 resulting in true!\n");
            return 0;
        }
        switch (idx) {
            case 0 :
                m = (*s - '0') * 10;
                break;
            case 1 :
                m += *s - '0';
                break;
            case 2 :
                if (*s != '/') {
                    //usage function
                    return 0;
                }
                break;
            case 3 :
                d = (*s - '0') * 10;
                break;
            case 4 :
                d += *s - '0';
                break;
            case 5 :
                if (*s != '/') {
                    //usage function
                    return 0;
                }
                break;
            case 6 :
                y = (*s - '0') * 1000;
                break;
            case 7 :
                y += (*s - '0') * 100;
                break;
            case 8 :
                y += (*s - '0') * 10;
                break;
            case 9 :
                y += *s - '0';
                break;
        }
        idx++;
    }

    if (m < 1 || m > 12 || d < 1 || d > 31 || y < 0 || y > 2022) {
        //usage function
        return 0;
    }
    else {
        myDate -> month = m;
        myDate -> day = d;
        myDate -> year = y;
        return 1;
    }
}

int cmpDate(const Date date1, const Date date2) {

    if (date1.year == date2.year && date1.month == date2.month && date1.day == date2.day) {
        return 0;
    }
    else if (date1.year < date2.year) {
        return -1;
    }
    else if (date1.year == date2.year && date1.month < date2.month) {
        return -1;
    }
    else if (date1.year == date2.year && date1.month == date2.month && date1.day < date2.day) {
        return -1;
    }
    else {
        return 1;
    }
}


// Part 2 Functions to implement
int genreComparator(void* str1, void* str2) {
    char *s1 = (char *)str1; 
    char *s2 = (char *)str2; 
    for (; *s1; s1++) {
        if (*s1 != *s2) {
            break;
        }
        s2++;
    } 
    if (*s1 == *s2)
        return 0;
    else if (*s1 < *s2)
        return -1;
    else if (*s1 > *s2)
        return 1;
}

void genrePrinter(void* data, void* fp, int flag) {
    FILE *output = (FILE *)fp;
    char *genre = (char *)data; //check if is: data node_t data pointer

    if (output == NULL) {
        printf("The file passed to genrePrinter is not open\n");
    }
    else if (output == stdout) {
        printf("%s ", genre);
    }
    else {
        fputs(genre, output);
        fputs("\n", output);
    }
}

void genreDeleter(void* data) {
    if (data == NULL) {
        // do nothing
    }
    else {
        free(data);
    }
}

list_t* getGenres(char* str) {
    list_t* genreList;
    char first[20], second[20], third[20], fourth[20], fifth[20], sixth[20];
    void *word;
    int result;

    //extract words from str, assign to void *word (pointer), pass genreList and word to InsertInOrder.
    char *s = str;

    result = sscanf(s, "%[^'|']|%[^'|']|%[^'|'}|%[^'|']|%[^'|']|%[^'|']|%s", first, second, third, fourth, fifth, sixth);
    printf("first: %s\n", first);
    printf("second: %s\n", second);
    printf("third: %s\n", third);
    printf("fourth: %s\n", fourth);
    printf("fifth: %s\n", fifth);
    printf("sixth: %s\n", sixth);

    printf("\nresult: %d\n", result);

    switch(result) {
        case 1 : 
            word = &first;
            InsertInOrder(genreList, word);
            break;
        case 2 : 
            word = &first;
            InsertInOrder(genreList, word);
            word = &second;
            InsertInOrder(genreList, word);
            break;
        case 3 : 
            word = &first;
            InsertInOrder(genreList, word);
            word = &second;
            InsertInOrder(genreList, word);
            word = &third;
            InsertInOrder(genreList, word);
            break;
        case 4 : 
            word = &first;
            InsertInOrder(genreList, word);
            word = &second;
            InsertInOrder(genreList, word);
            word = &third;
            InsertInOrder(genreList, word);
            word = &fourth;
            InsertInOrder(genreList, word);
            break;
        case 5 : 
            word = &first;
            InsertInOrder(genreList, word);
            word = &second;
            InsertInOrder(genreList, word);
            word = &third;
            InsertInOrder(genreList, word);
            word = &fourth;
            InsertInOrder(genreList, word);
            word = &fifth;
            InsertInOrder(genreList, word);
            break;
        case 6 : 
            word = &first;
            InsertInOrder(genreList, word);
            word = &second;
            InsertInOrder(genreList, word);
            word = &third;
            InsertInOrder(genreList, word);
            word = &fourth;
            InsertInOrder(genreList, word);
            word = &fifth;
            InsertInOrder(genreList, word);
            word = &sixth;
            InsertInOrder(genreList, word);
            break;
    }
    return genreList;
    return NULL; 
}

// Part 2 Generic Linked List functions
node_t* FindInList(list_t* list, void* token) {
    return NULL;
}

void DestroyList(list_t** list)  {

}




// Part 3 Functions to implement
void book_tPrinter(void* data, void *fp, int flag) {
 
}

int book_tISBNAscComparator(void* lhs, void* rhs) {
    return -999;
}

int book_tISBNDescComparator(void* lhs, void* rhs) {
    return -999;
}

void book_tDeleter(void* data) {

}

book_t* createBook(char* line) {
    return NULL;
}




// Part 4 Functions to implement
int bookMatch(book_t* curBook, search_t* criterion) {
    return -999;
}

void PrintNLinkedList(list_t* list, FILE* fp, int NUM) {
}