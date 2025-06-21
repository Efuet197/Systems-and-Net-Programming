// server_combined.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_CLIENTS 2

int client_sockets[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void broadcast_to_others(int sender_id, const char *message) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < client_count; i++) {
        if (i != sender_id) {
            send(client_sockets[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&lock);
}

void *client_handler(void *arg) {
    int client_id = *(int *)arg;
    char buffer[1024];
    int valread;

    while ((valread = read(client_sockets[client_id], buffer, sizeof(buffer))) > 0) {
        buffer[valread] = '\0';
        printf("Received from Client %d: %s", client_id + 1, buffer);
        broadcast_to_others(client_id, buffer);  // Real-time sync
    }

    close(client_sockets[client_id]);
    pthread_exit(NULL);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    pthread_t thread_id[MAX_CLIENTS];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, MAX_CLIENTS);

    printf("Server running on port %d...\n", PORT);
    printf("Waiting for %d clients to connect...\n", MAX_CLIENTS);

    while (client_count < MAX_CLIENTS) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        client_sockets[client_count] = new_socket;

        int *arg = malloc(sizeof(int));
        *arg = client_count;

        pthread_create(&thread_id[client_count], NULL, client_handler, arg);
        client_count++;
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(thread_id[i], NULL);
    }

    close(server_fd);
    return 0;
}