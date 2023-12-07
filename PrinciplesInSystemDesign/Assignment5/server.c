#include <sys/types.h>  // getaddrinfo(), setsockopt(), bind(), listen(), accept(), recv(), send()
#include <sys/socket.h> // getaddrinfo(), setsockopt(), bind(), listen(), accept(), recv(), send()
#include <netdb.h>      // getaddrinfo()
#include <string.h>     // memset()
#include <stdio.h>      // perror()
#include <stdlib.h>     // exit()
#include <unistd.h>     // close()
#include <math.h>       // round()
#define arrsize 512

typedef struct {
    double price;
    char date[11];
} market_t;

void prep_msg(char *msg_ptr, int strsize, char *msg); 
char* mod_date(char *old_date);
void ready_msg(char *msg, char *literal);
int print_prices(market_t arr1[], market_t arr2[], int arr_size, char *date, char *msg); 
int print_max(market_t arr1[], market_t arr2[], int arr_size, char *type, char *name, char *start, char *end, char *msg);
float find_max_gain_in_range(market_t arr[], int arr_size, char *start, char *end); 
float find_max_loss_in_range(market_t arr[], int arr_size, char *start, char *end); 

int main(int argc, char *argv[]) {

    FILE *stream_pfe = fopen(argv[1], "r");
    if (stream_pfe == NULL) {
        perror("error");
        return (EXIT_FAILURE);
    }
    FILE *stream_mrna = fopen(argv[2], "r");
    if (stream_mrna == NULL) {
        perror("error");
        return (EXIT_FAILURE);
    }

    double close_price;
    char *date_str = NULL;
    char *token1 = NULL;
    char pfe_buf[256] = {0};
    char *pfe_ptr = pfe_buf;
    char mrna_buf[256] = {0};
    char *mrna_ptr = mrna_buf;
    market_t pfe_array[arrsize] = {0};
    market_t mrna_array[arrsize] = {0};
    int i, j;

    fgets(pfe_buf, 256, stream_pfe); // discard first line
    i = 0;
    while (fgets(pfe_buf, 256, stream_pfe)) {
        pfe_ptr = pfe_buf;
        j = 0;
        while ((token1 = strsep(&pfe_ptr, ",")) != NULL) {
            if (j == 0) {
                strcpy(pfe_array[i].date, mod_date(token1));
            }
            if (j == 4) {
                close_price = atof(token1);
                close_price = round(close_price * 100.0)/100.0;
                pfe_array[i].price = close_price;
            }
            ++j;
        } 
        ++i;
    }

    fgets(mrna_buf, 256, stream_mrna); // discard first line
    i = 0;
    while (fgets(mrna_buf, 256, stream_mrna)) {
        mrna_ptr = mrna_buf;
        j = 0;
        while ((token1 = strsep(&mrna_ptr, ",")) != NULL) {
            if (j == 0) {
                strcpy(mrna_array[i].date, mod_date(token1));
            }
            if (j == 4) {
                close_price = atof(token1);
                close_price = round(close_price * 100.0)/100.0;
                mrna_array[i].price = close_price;
            }
            ++j;
        }  
        ++i;
    }

    const char *portNumber = argv[3];
    char buffer[256] = {0};
    char *buf_ptr = buffer;
    char msg[256] = {0};
    char *token2 = NULL, *token3 = NULL, *token4 = NULL, *token5 = NULL;
    const int backlog = 1;
    int serverSocket = 0;
    struct addrinfo hints;
    struct addrinfo *results;
    struct addrinfo *record;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(NULL, portNumber, &hints, &results) != 0) {
        perror("error, not a");
        exit(EXIT_FAILURE);
    }

    for (record = results; record != NULL; record = record->ai_next) {
        serverSocket = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
        if (serverSocket == -1) continue;
        int enable = 1;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        if (bind(serverSocket, record->ai_addr, record->ai_addrlen) == 0) break;
        close(serverSocket);
    }

    if (record == NULL) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(results);

    if (listen(serverSocket, backlog) == -1) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    printf("*** Server Started ***\n");

    while (1) {

        int clientSocket;
        struct sockaddr clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);

        if ((clientSocket = accept(serverSocket, &clientAddress, &clientAddressLength)) < 0) {
            perror("error");
            exit(EXIT_FAILURE);
        }

        //****************** recv & send ********************// 
        memset(buffer, 0, sizeof(buffer));
        
        if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1) {
            perror("error");
            exit(EXIT_FAILURE);
        }

        buf_ptr = buffer;
        buf_ptr++; // point to buffer[1] 
        printf("%s\n", buf_ptr);

        token2 = strsep(&buf_ptr, " \0");
        if (strcmp(token2, "PricesOnDate") != 0 && strcmp(token2, "MaxPossible") != 0) {
            ready_msg(msg, "Invalid Syntax");
        }
        else if (strcmp(token2, "PricesOnDate") == 0) {
            token2 = strsep(&buf_ptr, " \0");
            if (token2 == NULL || *token2 == '\0') {
                ready_msg(msg, "Invalid Syntax");
            }
            else if (print_prices(pfe_array, mrna_array, arrsize, token2, msg) == 0) {
                ready_msg(msg, "Unknown");
            }
        }
        else if (strcmp(token2, "MaxPossible") == 0) {
            token2 = strsep(&buf_ptr, " \0");
            if (token2 != NULL && *token2 != '\0') {
                token3 = strsep(&buf_ptr, " \0");
                if (token3 != NULL && *token3 != '\0') {
                    token4 = strsep(&buf_ptr, " \0");
                    if (token4 != NULL && *token4 != '\0') {
                        token5 = strsep(&buf_ptr, " \0");
                        if (token5 != NULL && *token5 != '\0') {
                            if (print_max(pfe_array, mrna_array, arrsize, token2, token3, token4, token5, msg) == 0) {
                                ready_msg(msg, "Unknown");
                            }
                        }
                        else {
                            ready_msg(msg, "Invalid Syntax");
                        }
                    }
                    else {
                        ready_msg(msg, "Invalid Syntax");
                    }
                }
                else {
                    ready_msg(msg, "Invalid Syntax");
                }
            }
            else {
                ready_msg(msg, "Invalid Syntax");
            }
        }

        if (send(clientSocket, msg, sizeof(msg), 0) == -1) {
            perror("error");
            exit(EXIT_FAILURE);
        }

        close(clientSocket);
    }

    fclose(stream_pfe);
    fclose(stream_mrna);
    return 0;
}

void ready_msg(char *msg, char *literal) {
    char msg_buf[256] = {0};
    int msg_size = 0;

    strcpy(msg_buf, literal);
    msg_size = strlen(msg_buf);
    prep_msg(msg_buf, msg_size, msg);
}

int print_prices(market_t arr1[], market_t arr2[], int arr_size, char *date, char *msg) {
    char msg_buf[256] = {0};
    int msg_size = 0;

    for (int i = 0; i < arr_size; ++i) {
        if (strcmp(date, arr1[i].date) == 0) {
            sprintf(msg_buf, "PFE: %.2f | MRNA: %.2f", arr1[i].price, arr2[i].price);
            msg_size = strlen(msg_buf);
            prep_msg(msg_buf, msg_size, msg);
            return 1;
        }
    }
    return 0;
}

void prep_msg(char *msg_ptr, int strsize, char *msg) {
    msg[0] = strsize; 
    for (int i = 1; i <= strsize; ++i) {
        msg[i] = *msg_ptr;
        msg_ptr++;
    }
    msg[strsize + 1] = '\0';
}

char* mod_date(char *old_date) {
    static char new_date[11];
    char *new_date_ptr = new_date;
    strcpy(new_date_ptr, old_date);
    char month[3]; 
    char day[3];
    char year[5];

    strcpy(month, strsep(&new_date_ptr, "/")); 
    strcpy(day, strsep(&new_date_ptr, "/")); 
    strcpy(year, strsep(&new_date_ptr, "/")); 

    if (strlen(day) < 2) {
        char temp = day[0];
        day[0] = '0';
        day[1] = temp;
        day[2] = '\0';
    }

    if (strlen(month) < 2) {
        char temp = month[0];
        month[0] = '0';
        month[1] = temp;
        month[2] = '\0';
    }

    sprintf(new_date,"%s-%s-%s", year, month, day);

    return new_date;
}

int print_max(market_t arr1[], market_t arr2[], int arr_size, char *type, char *name, char *start, char *end, char *msg) {
    char msg_buf[256] = {0};
    int msg_size = 0;
    float max_profit = 0, max_loss = 0;

    if (strcmp(type, "profit") == 0) {
        if (strcmp(name, "PFE") == 0) {
            max_profit = find_max_gain_in_range(arr1, arr_size, start, end);
            if (max_profit >= 0) {
                sprintf(msg_buf, "%.2f", max_profit);
                msg_size = strlen(msg_buf);
                prep_msg(msg_buf, msg_size, msg);
                return 1;
            }
            else {
                return 0;
            }
        }
        else if (strcmp(name, "MRNA") == 0) {
            max_profit = find_max_gain_in_range(arr2, arr_size, start, end);
            if (max_profit >= 0) {
                sprintf(msg_buf, "%.2f", max_profit);
                msg_size = strlen(msg_buf);
                prep_msg(msg_buf, msg_size, msg);
                return 1;
            }
            else {
                return 0;
            }
        }
        else {
            ready_msg(msg, "Invalid Syntax");
        }
    }
    else if (strcmp(type, "loss") == 0) {
        if (strcmp(name, "PFE") == 0) {
            max_loss = find_max_loss_in_range(arr1, arr_size, start, end);
            if (max_loss >= 0) {
                sprintf(msg_buf, "%.2f", max_loss);
                msg_size = strlen(msg_buf);
                prep_msg(msg_buf, msg_size, msg);
                return 1;
            }
            else {
                return 0;
            }
        }
        else if (strcmp(name, "MRNA") == 0) {
            max_loss = find_max_loss_in_range(arr2, arr_size, start, end);
            if (max_loss >= 0) {
                sprintf(msg_buf, "%.2f", max_loss);
                msg_size = strlen(msg_buf);
                prep_msg(msg_buf, msg_size, msg);
                return 1;
            }
            else {
                return 0;
            }
        }
        else {
            ready_msg(msg, "Invalid Syntax");
        }
    }
    else {
        ready_msg(msg, "Invalid Syntax");
    }

    return 0;
}

float find_max_gain_in_range(market_t arr[], int arr_size, char *start, char *end) {
    float max_gain = 0.0, diff = 0.0;
    int start_index, end_index, i, j;

    for (start_index = 0; start_index < arr_size - 1; ++start_index) {
        if (strcmp(arr[start_index].date, start) != 0) {
            continue;
        }
        else {
            break;
        }
    }
    if (start_index == arr_size - 1) {
        return 0;
    }
    
    for (end_index = start_index + 1; end_index < arr_size; ++end_index) {
        if (strcmp(arr[end_index].date, end) != 0) {
            continue;
        }
        else {
            break;
        }
    }
    if (end_index == arr_size) {
        return 0;
    }

    for (i = start_index; i < end_index; ++i) {
        for (j = i + 1; j <= end_index; ++j) {
            diff = arr[j].price - arr[i].price;
            if (diff > max_gain) {
                max_gain = diff;
            }
        }
    }
    return max_gain;
}

float find_max_loss_in_range(market_t arr[], int arr_size, char *start, char *end) {
    float max_loss = 0.0, diff = 0.0;
    int start_index, end_index, i, j;

    for (start_index = 0; start_index < arr_size - 1; ++start_index) {
        if (strcmp(arr[start_index].date, start) != 0) {
            continue;
        }
        else {
            break;
        }
    }
    if (start_index == arr_size - 1) {
        return 0;
    }
    
    for (end_index = start_index + 1; end_index < arr_size; ++end_index) {
        if (strcmp(arr[end_index].date, end) != 0) {
            continue;
        }
        else {
            break;
        }
    }
    if (end_index == arr_size) {
        return 0;
    }

    for (i = start_index; i < end_index; ++i) {
        for (j = i + 1; j <= end_index; ++j) {
            diff = arr[i].price - arr[j].price;
            if (diff > max_loss) {
                max_loss = diff;
            }
        }
    }
    return max_loss;
}