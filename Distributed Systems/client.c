// client_combined.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int sock = 0;

void *receive_handler(void *arg) {
    char buffer[1024];
    int valread;

    while ((valread = read(sock, buffer, sizeof(buffer))) > 0) {
        buffer[valread] = '\0';
        printf("\n[Incoming]: %s", buffer);
        fflush(stdout);
    }

    return NULL;
}

int main() {
    struct sockaddr_in serv_addr;
    char buffer[1024];
    pthread_t recv_thread;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.1.174");  // Change if using remote server

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("Connected to server. You can start typing messages:\n");

    pthread_create(&recv_thread, NULL, receive_handler, NULL);

    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        send(sock, buffer, strlen(buffer), 0);
    }

    close(sock);
    return 0;
}