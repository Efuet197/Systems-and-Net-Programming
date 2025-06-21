// fifo_server.c
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
    char buffer[BUFFER_SIZE];

    // Create the FIFOs if they don’t exist
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    printf("Server: Waiting for client message...\n");

    // Open FIFO1 for reading
    int fd_read = open(FIFO1, O_RDONLY);
    if (fd_read < 0) {
        perror("Failed to open FIFO1 for reading");
        exit(1);
    }

    // Read message from client
    read(fd_read, buffer, BUFFER_SIZE);
    printf("Server: Received from client: %s\n", buffer);
    close(fd_read);

    // Process message (simple echo + text)
    strcat(buffer, " [ACK from server]");

    // Open FIFO2 for writing
    int fd_write = open(FIFO2, O_WRONLY);
    if (fd_write < 0) {
        perror("Failed to open FIFO2 for writing");
        exit(1);
    }

    // Send processed message to client
    write(fd_write, buffer, strlen(buffer) + 1);
    printf("Server: Response sent to client.\n");
    close(fd_write);

    return 0;
}
