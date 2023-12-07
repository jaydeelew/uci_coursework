#include <sys/types.h>  // getaddrinfo(), recv(), send()
#include <sys/socket.h> // getaddrinfo(), recv(), send() 
#include <netdb.h>      // getaddrinfo()
#include <string.h>     // memset()
#include <stdio.h>      // perror()
#include <stdlib.h>     // exit()
#include <unistd.h>     // close()

int main(int argc, char *argv[]) {

    const char *hostName = argv[1];
    const char *portNumber = argv[2];
    char buffer[256] = {0};
    char *buffer_ptr = buffer;
    char *token;
    int client_fd, err;
    struct addrinfo hints;
    struct addrinfo *results;
    struct addrinfo *record;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if ((getaddrinfo(hostName, portNumber, &hints, &results)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        exit(EXIT_FAILURE);
    }

    for (record = results; record != NULL; record = record->ai_next) {
        client_fd = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
        if (client_fd == -1) continue;
        if (connect(client_fd, record->ai_addr, record->ai_addrlen) != -1) break;
        close(client_fd);
    }

    if (record == NULL) {
        fprintf(stderr, "Could not create client socket or could not connect\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(results);

    /*****************************send & recv*******************************/ 
    while (1) {
        printf("> ");
        fgets(buffer, 256, stdin);
        buffer_ptr = buffer;
        token = strsep(&buffer_ptr, "\n");
        int tokensize = strlen(token) + 1;
        // NULL
        if (*token == '\0') {
            printf("Invalid Syntax\n");
            continue;
        }
        // quit
        if (strcmp(token, "quit") == 0) {
            break;
        }

        if (send(client_fd, token, tokensize, 0) == -1) {
            fprintf(stderr, "Failed to send message to server\n");
            exit(EXIT_FAILURE);
        }

        memset(buffer, 0, sizeof(buffer));
        // wait to receive message from server
        if (recv(client_fd, buffer, sizeof(buffer), 0) == -1) {
            fprintf(stderr, "Failed to receive message from server\n");
            exit(EXIT_FAILURE);
        }

        // message from server 
        if (buffer[0] != '\0') {
            printf("%s\n", &buffer[0]);
        }
    }

    close(client_fd);

    return 0;
}