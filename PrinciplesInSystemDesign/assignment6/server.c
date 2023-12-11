#include <sys/types.h>  // getaddrinfo(), setsockopt(), bind(), listen(), accept(), recv(), send()
#include <sys/socket.h> // getaddrinfo(), setsockopt(), bind(), listen(), accept(), recv(), send()
#include <netdb.h>      // getaddrinfo()
#include <string.h>     // memset()
#include <stdio.h>      // perror()
#include <stdlib.h>     // exit()
#include <unistd.h>     // close()
#include <pthread.h>    // pthread_create(), pthread_detach(), pthread_self()
#include <semaphore.h>  // sem_init(), sem_wait(), sem_post()
#include "Md5.c"

void *thread(void *void_ptr); 

typedef struct sockaddr sa;

typedef struct {
    int listen_fd;
    int connect_fd;
    struct sockaddr_storage client_address;
    socklen_t client_address_len;
} fd_t;

typedef struct {
    char name[128];
    FILE *stream;
} stream_t;

typedef struct {
    char *name;
    char mode; // either 'r' or 'a'
} opens_t;

//globals
opens_t *openfiles[4]; // trusting that max clients is 4
int openfiles_cnt = 0;
sem_t mutex;

int main(int argc, char *argv[]) {
    const char *portNumber = argv[1];
    char *token;
    const int backlog = 1;
    int listen_fd, err;
    struct addrinfo hints;
    struct addrinfo *results;
    struct addrinfo *record;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    fd_t connect_fd;    
    fd_t *connect_fd_ptr;
    socklen_t client_address_len = 0;
    struct sockaddr_storage client_address;
    pthread_t tid; 

    sem_init(&mutex, 0, 1);

    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (err = getaddrinfo(NULL, portNumber, &hints, &results) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        exit(EXIT_FAILURE);
    }
    // try list of address structures if/until it binds, else close socket file descriptor
    for (record = results; record != NULL; record = record->ai_next) {
        listen_fd = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
        if (listen_fd == -1) continue;
        int enable = 1;
        setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        if (bind(listen_fd, record->ai_addr, record->ai_addrlen) == 0) break;
        close(listen_fd);
    }

    if (record == NULL) {
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(results);

    if (listen(listen_fd, backlog) == -1) {
        fprintf(stderr, "Could not start socket listen\n");
        exit(EXIT_FAILURE);
    }

    printf("*** Server Started ***\n");

    while (1) {
        connect_fd_ptr = malloc(sizeof(connect_fd));

        connect_fd_ptr->listen_fd = listen_fd;
        connect_fd_ptr->client_address = client_address;
        connect_fd_ptr->client_address_len = client_address_len;

        if ((connect_fd_ptr->connect_fd = accept(listen_fd, (sa*)&client_address, &client_address_len)) < 0) {
            fprintf(stderr, "Failed to accept client socket\n");
            exit(EXIT_FAILURE);
        }

        pthread_create(&tid, NULL, thread, connect_fd_ptr);
    }
}

/*****************************thread*******************************/ 
void *thread(void *void_ptr) {  
    int listen_fd = ((fd_t*)void_ptr)->listen_fd; 
    int connect_fd = ((fd_t*)void_ptr)->connect_fd; 
    struct sockaddr_storage client_address = ((fd_t*)void_ptr)->client_address;
    socklen_t client_address_len = ((fd_t*)void_ptr)->client_address_len;

    pthread_detach(pthread_self());
    free(void_ptr); // frees connect_fd_ptr
    char buffer[256] = {0};
    char msg[256] = {0};
    char *buf_ptr = buffer;
    char *token = NULL;
    FILE *stream = NULL;
    int byanother = 0, closed = 0, readcount = 0, i = 0;
    unsigned char digest[16] = {0};
    unsigned char *digest_ptr = digest;
    opens_t *temp;
    stream_t openread = {
        {0},
        NULL
    };
    stream_t *openreadptr = &openread;
    stream_t openappend = {
        {0},
        NULL
    };
    stream_t *openappendptr = &openappend;   

    /*****************************recv & send*******************************/ 
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        memset(msg, 0, sizeof(msg));
        memset(digest, 0, sizeof(digest));
  
        if (recv(connect_fd, buffer, sizeof(buffer), 0) == -1) {
            fprintf(stderr, "Failed to receive message from client\n");
            break; 
        }

        buf_ptr = buffer;
        if (*buf_ptr == '\0') { // client sends empty msg when terminating
            if (openreadptr->stream != NULL) { // if a file is open for reading
                fclose(openreadptr->stream);
            }
            if (openappendptr->stream != NULL) { // if a file is open for appending
                fclose(openappendptr->stream);
            }

            sem_wait(&mutex);
            for (int i = 0; i < openfiles_cnt; ++i) {
                if ((strcmp(openfiles[i]->name, openreadptr->name) == 0) || (strcmp(openfiles[i]->name, openappendptr->name) == 0)) {
                    free(openfiles[i]->name);
                    free(openfiles[i]);
                    for (; i < openfiles_cnt; ++i) {
                        (openfiles[i] = openfiles[i + 1]);
                    }
                    openfiles_cnt--;
                    break;
                }
            }
            sem_post(&mutex);

            break;
        }
        else {
            printf("%s\n", buf_ptr);
        }

        token = strsep(&buf_ptr, " \0");

        if (strcmp(token, "openRead") == 0) {
            if (openappendptr->stream != NULL || openreadptr->stream != NULL) {
                strcpy(msg, "A file is already open");
            }
            else {
                token = strsep(&buf_ptr, " \0");
                if (token == NULL || *token == '\0') {
                    strcpy(msg, "A file name must be entered");
                }           
                else {
                    byanother = 0;

                    sem_wait(&mutex);
                    for (int i = 0; i < openfiles_cnt; ++i) {
                        if ((strcmp(openfiles[i]->name, token) == 0) && (openfiles[i]->mode == 'a')) {
                            strcpy(msg, "The file is open for appending by another client");
                            byanother = 1;
                            break;
                        }
                    }
                    if (byanother == 0) {
                        stream = fopen(token, "r");
                        if (stream == NULL) {
                            strcpy(msg, "Failed to open file");
                        }
                        else {
                            strcpy(openreadptr->name, token);
                            openreadptr->stream = stream;

                            openfiles[openfiles_cnt] = malloc(sizeof(opens_t));
                            openfiles[openfiles_cnt]->name = malloc(strlen(token) + 1); 
                            strcpy(openfiles[openfiles_cnt]->name, token);
                            openfiles[openfiles_cnt]->mode = 'r';
                            openfiles_cnt++;
                        }
                    }
                    sem_post(&mutex);
                }
            }
        }
        else if (strcmp(token, "openAppend") == 0) {
            if (openappendptr->stream != NULL || openreadptr->stream != NULL) {
                strcpy(msg, "A file is already open");
            }
            else {
                token = strsep(&buf_ptr, " \0");
                if (token == NULL || *token == '\0') {
                    strcpy(msg, "A file name must be entered");
                }
                else {           
                    byanother = 0;

                    sem_wait(&mutex);
                    for (int i = 0; i < openfiles_cnt; ++i) {
                        if (strcmp(openfiles[i]->name, token) == 0) {
                            strcpy(msg, "The file is open by another client");
                            byanother = 1;
                            break;
                        }
                    }
                    if (byanother == 0) {
                        stream = fopen(token, "r");
                        if (stream == NULL) {
                            strcpy(msg, "Failed to open file");
                        }
                        else {
                            fclose(stream);
                            stream = fopen(token, "a");
                            strcpy(openappendptr->name, token);
                            openappendptr->stream = stream;

                            openfiles[openfiles_cnt] = malloc(sizeof(opens_t));
                            openfiles[openfiles_cnt]->name = malloc(strlen(token) + 1); 
                            strcpy(openfiles[openfiles_cnt]->name, token);
                            openfiles[openfiles_cnt]->mode = 'a';
                            openfiles_cnt++;
                        }
                    }
                    sem_post(&mutex);
                }
            }
        }
        else if (strcmp(token, "read") == 0) {
            token = strsep(&buf_ptr, " \0");
            if (token == NULL || *token == '\0') {
                strcpy(msg, "Number of characters must be entered");
            }
            else if (atoi(token) < 1 || atoi(token) > 200) {
                strcpy(msg, "Number of characters ranging from 1 to 200 must be entered");
            }
            else if (openreadptr->stream != NULL) {
                fread(msg, sizeof(char), atoi(token), openread.stream);
            }
            else {
                strcpy(msg, "File not open for reading");
            }
        }
        else if (strcmp(token, "append") == 0) {
            token = strsep(&buf_ptr, "\0");
            if (token == NULL || *token == '\0') {
                strcpy(msg, "String to append must be entered");
            }
            else if (strlen(token) > 200) {
                strcpy(msg, "String must be 200 characters or less");
            }
            else if (openappendptr->stream != NULL) {
                fprintf(openappend.stream, "%s", token);
                fflush(stdout);
                fflush(openappend.stream);
            }
            else {
                strcpy(msg, "File not open for appending");
            }
        }
        else if (strcmp(token, "close") == 0) {
            closed = 0;
            token = strsep(&buf_ptr, "\0");
            if (token == NULL || *token == '\0') {
                strcpy(msg, "A file name must be entered");
            } 
            else {
                if (openreadptr->stream != NULL) { // if a file is open for reading
                    if (strcmp(openreadptr->name, token) == 0) {
                        fclose(openreadptr->stream);
                        memset(openreadptr->name, 0, sizeof(openreadptr->name));
                        openreadptr->stream = NULL; 

                        sem_wait(&mutex);
                        for (int i = 0; i < openfiles_cnt; ++i) {
                            if (strcmp(openfiles[i]->name, token) == 0) {
                                free(openfiles[i]->name);
                                free(openfiles[i]);
                                for (; i < openfiles_cnt; ++i) {
                                    (openfiles[i] = openfiles[i + 1]);
                                }
                                openfiles_cnt--;
                                break;
                            }
                        }
                        sem_post(&mutex);

                        closed = 1;
                    }
                }
                if (openappendptr->stream != NULL) { // if a file is open for appending
                    if (strcmp(openappendptr->name, token) == 0) {
                        fclose(openappendptr->stream);
                        memset(openappendptr->name, 0, sizeof(openappendptr->name));
                        openappendptr->stream = NULL; 

                        sem_wait(&mutex);
                        for (int i = 0; i < openfiles_cnt; ++i) {
                            if (strcmp(openfiles[i]->name, token) == 0) {
                                free(openfiles[i]->name);
                                free(openfiles[i]);
                                for (; i < openfiles_cnt; ++i) {
                                    (openfiles[i] = openfiles[i + 1]);
                                }
                                openfiles_cnt--;
                                break;
                            }
                        }
                        sem_post(&mutex);

                        closed = 1;
                    }
                }
                if (closed == 0) {
                    strcpy(msg, "Can not close, the file is not open by this client");
                }
            }
        }
        else if (strcmp(token, "getHash") == 0) {
            token = strsep(&buf_ptr, " \0");
            if (token == NULL || *token == '\0') {
                strcpy(msg, "A file name must be entered");
            }
            else if (strcmp(openappend.name, token) == 0) {
                strcpy(msg, "The file is open for appending by this client");
            }
            else {
                byanother = 0;

                sem_wait(&mutex);
                for (int i = 0; i < openfiles_cnt; ++i) {
                    if ((strcmp(openfiles[i]->name, token) == 0) && (openfiles[i]->mode == 'a')) {
                        strcpy(msg, "The file is open for appending by another client");
                        byanother = 1;
                        break;
                    }
                }
                sem_post(&mutex);

                if (byanother == 0) {
                    stream = fopen(token, "r");
                    if (stream == NULL) {
                        strcpy(msg, "File not found");
                    }
                    else {
                        fclose(stream);
                        i = 0;

                        sem_wait(&mutex);
                        MDFile(token, digest);
                        sem_post(&mutex);

                        digest_ptr = &digest[0]; // move ptr to beginning of digest
                        while(i < 32)
                        {
                            sprintf(&msg[i], "%02x", *digest_ptr);
                            digest_ptr++;
                            i += 2;
                        }
                    }
                }
            }
        }       
        else if (strcmp(token, "printOpen") == 0) { // REMOVE AFTER TESTING
            if (openfiles_cnt > 0) {
                sem_wait(&mutex);
                for (int i = 0; i < openfiles_cnt; ++i) {
                    strcat(msg, openfiles[i]->name);
                    strcat(msg, "(");
                    strcat(msg, &openfiles[i]->mode);
                    strcat(msg, ")");
                    strcat(msg, " ");
                }
                sem_post(&mutex);
            }
            else {
                strcpy(msg, "There are no open files");
            }
        }
        else if (strcmp(token, "printThis") == 0) { // REMOVE AFTER TESTING
            if (openreadptr->stream != NULL) { // if a file is open for reading
                strcat(msg, openreadptr->name);
                strcat(msg, "(r)");
            }
            else if (openappendptr->stream != NULL) { // if a file is open for appending
                strcat(msg, openappendptr->name);
                strcat(msg, "(a)");
            }
            else {
                strcpy(msg, "There are no open files");
            }
        }
        else {
            strcpy(msg, "An invalid command was entered");
        }
        
        if (send(connect_fd, msg, sizeof(msg), 0) == -1) {
            fprintf(stderr, "Failed to send message to client\n");
            exit(EXIT_FAILURE);
        }
    }

    close(connect_fd);
    return NULL;
}