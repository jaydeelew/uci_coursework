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
    char cmdbuf[256] = {0};
    char *cmdbuf_ptr = cmdbuf;
    char buffer[256] = {0};
    char *token;
    int clientSocket, strsize;
    struct addrinfo hints;
    struct addrinfo *results;
    struct addrinfo *record;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    printf("*** Client Started ***\n");

    while (1) {
        printf("> ");
        fgets(cmdbuf, 256, stdin);
        cmdbuf_ptr = cmdbuf;
        token = strsep(&cmdbuf_ptr, "\n");
        // NULL
        if (*token == '\0') {
            printf("Invalid Syntax\n");
            continue;
        }
        // quit
        if (strcmp(token, "quit") == 0) {
            return 0;
        }

        for (strsize = 0; cmdbuf[strsize] != '\0'; strsize++) {} 

        if ((getaddrinfo(hostName, portNumber, &hints, &results)) != 0) {
            perror("error: ");
            exit(EXIT_FAILURE);
        }

        for (record = results; record != NULL; record = record->ai_next) {
            clientSocket = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
            if (clientSocket == -1) continue;
            if (connect(clientSocket, record->ai_addr, record->ai_addrlen) != -1) break;
            close(clientSocket);
        }

        if (record == NULL) {
            perror("error: ");
            exit(EXIT_FAILURE);
        }

        freeaddrinfo(results);

        //******************send & recv********************// 
        char msg[strsize + 2]; // string + size byte + null terminator
        msg[0] = strsize; 
        for (int i = 1; i <= strsize; ++i) {
            msg[i] = cmdbuf[i - 1];
        }
        msg[strsize + 1] = '\0';

        if (send(clientSocket, msg, strsize + 2, 0) == -1) { // chg size to sizeof(msg)?
            perror("error: ");
            exit(EXIT_FAILURE);
        }

        memset(buffer, 0, sizeof(buffer));
        // wait to receive message from server
        if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1) {
            perror("error: ");
            exit(EXIT_FAILURE);
        }
        // message from server 
        printf("%s\n", &buffer[1]);

        close(clientSocket);
    }

    return 0;
}