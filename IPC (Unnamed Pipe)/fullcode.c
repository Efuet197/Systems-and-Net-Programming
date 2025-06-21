#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 100

int main() {
    int pipefd1[2]; // Pipe from parent to child
    int pipefd2[2]; // Pipe from child to parent
    pid_t pid;
    char write_msg[] = "Hello from Parent!";
    char read_buffer[BUFFER_SIZE];

    // Create the pipes
    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent Process
        close(pipefd1[0]); // Close read end of pipe1
        close(pipefd2[1]); // Close write end of pipe2

        // Send message to child
        write(pipefd1[1], write_msg, strlen(write_msg) + 1);
        printf("Parent: Sent to child -> %s\n", write_msg);

        // Wait for response from child
        read(pipefd2[0], read_buffer, BUFFER_SIZE);
        printf("Parent: Received from child -> %s\n", read_buffer);

        close(pipefd1[1]);
        close(pipefd2[0]);
    } else {
        // Child Process
        close(pipefd1[1]); // Close write end of pipe1
        close(pipefd2[0]); // Close read end of pipe2

        // Read message from parent
        read(pipefd1[0], read_buffer, BUFFER_SIZE);
        printf("Child: Received from parent -> %s\n", read_buffer);

        // Send response to parent
        char response[] = "Hello from Child!";
        write(pipefd2[1], response, strlen(response) + 1);
        printf("Child: Sent to parent -> %s\n", response);

        close(pipefd1[0]);
        close(pipefd2[1]);
    }

    return 0;
}
    