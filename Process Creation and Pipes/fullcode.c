

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int numClients;
    int childCount = 0;

    printf("Enter the number of clients to handle: ");
    scanf("%d", &numClients);

    for (int i = 0; i < numClients; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            continue; // Skip this iteration, try the next
        }

        if (pid == 0) {
            // Child process simulating a client
            printf("\n[Client] Handling request...\n");
            printf("[Client] PID: %d, Parent PID: %d\n", getpid(), getppid());
            sleep(2); // Simulated processing time
            printf("[Client] PID %d completed its task.\n", getpid());
            exit(0); // Important: Exit to prevent child from forking more clients
        } else {
            // Parent process
            childCount++;
        }
    }

    // Parent waits for all children
    for (int i = 0; i < childCount; i++) {
        int status;
        pid_t finishedPid = wait(&status);
        printf("[Server] Client process with PID %d has finished.\n", finishedPid);
    }

    printf("\n[Server] Total client processes handled: %d\n", c
