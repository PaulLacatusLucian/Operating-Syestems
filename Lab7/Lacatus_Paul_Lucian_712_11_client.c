#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_NAME "/tmp/my_pipe"
#define BUFFER_SIZE 256

int main()
{
    int pipe_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_written;

    // FIFO in write-only offnen
    pipe_fd = open(FIFO_NAME, O_WRONLY);
    if (pipe_fd == -1) {
        perror("Failed to open FIFO pipe");
        exit(EXIT_FAILURE);
    }

    // directory Name lesen
    printf("Enter the directory name: ");
    fgets(buffer, BUFFER_SIZE, stdin);

    // es zum server schicken
    bytes_written = write(pipe_fd, buffer, strlen(buffer));
    if (bytes_written == -1) {
        perror("Failed to write to FIFO pipe");
        exit(EXIT_FAILURE);
    }

    // der pipe schliessen
    close(pipe_fd);

    // offnen der FIFO pipe in read-only fur total bytes
    pipe_fd = open(FIFO_NAME, O_RDONLY);
    if (pipe_fd == -1) {
        perror("Failed to open FIFO pipe");
        exit(EXIT_FAILURE);
    }

    // total bytes von server lesen
    unsigned long long total_bytes;
    ssize_t bytes_read = read(pipe_fd, &total_bytes, sizeof(total_bytes));
    if (bytes_read == -1) {
        perror("Failed to read from FIFO pipe");
        exit(EXIT_FAILURE);
    }

    // pipe schliessen
    close(pipe_fd);

    printf("Total bytes in directory: %llu\n", total_bytes);

    return 0;
}
