// fifo_client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define BUFFER_SIZE 100

int main() {
    char buffer[BUFFER_SIZE] = "Hello from client";

    // Open FIFO1 for writing
    int fd_write = open(FIFO1, O_WRONLY);
    if (fd_write < 0) {
        perror("Failed to open FIFO1 for writing");
        exit(1);
    }

    // Send message to server
    write(fd_write, buffer, strlen(buffer) + 1);
    printf("Client: Sent message to server.\n");
    close(fd_write);

    // Open FIFO2 for reading response
    int fd_read = open(FIFO2, O_RDONLY);
    if (fd_read < 0) {
        perror("Failed to open FIFO2 for reading");
        exit(1);
    }

    // Read response from server
    read(fd_read, buffer, BUFFER_SIZE);
    printf("Client: Received from server: %s\n", buffer);
    close(fd_read);

    return 0;
}
