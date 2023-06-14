#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

int isPrime(int n) {
    if (n <= 1) {
        return 0;
    }
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    pid_t pid;
    int num;
    int i = 2;
    printf("Enter an integer: ");
    scanf("%d", &num);

    while (num > 1 && i <= num) {
        if (isPrime(i) && num % i == 0) {
            int pipefd[2];
            if (pipe(pipefd) == -1) {
                fprintf(stderr, "Pipe failed\n");
                return 1;
            }

            pid = fork();
            if (pid < 0) {
                fprintf(stderr, "Fork failed\n");
                return 1;
            } else if (pid == 0) {
                close(pipefd[0]); //inchid read end-ul

                int count = 0;
                int writeNumber = num;
                while (num % i == 0) {
                    num /= i;
                    count++;
                }
                printf("Child process: Divided %d by %d %d times\n", writeNumber, i, count);
                write(pipefd[1], &num, sizeof(num));
                close(pipefd[1]); // inchid write end-ul
                exit(0);
            } else {
                printf("Parent process: Created child process with pid %d\n", pid);
                int childStatus;
                wait(&childStatus);

                close(pipefd[1]); // inchid write end-ul

			
                read(pipefd[0], &num, sizeof(num)); // citesc valorile
                close(pipefd[0]); // inchide read end-ul

                continue;
            }
        }
        i++;
    }

    return 0;
}
