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
    ssize_t bytes_read;

    // eine FIFO pipe erstellen
    mkfifo(FIFO_NAME, 0666);

    // es in read-only mode offnen
    pipe_fd = open(FIFO_NAME, O_RDONLY);
    if (pipe_fd == -1) {
        perror("Failed to open FIFO pipe");
        exit(EXIT_FAILURE);
    }

    // Dateiname von der Client lesen
    bytes_read = read(pipe_fd, buffer, BUFFER_SIZE);
    buffer[bytes_read - 1] = '\0';

    // eine neue temporare Datei erstellen fur ls -l
    char temp_file[] = "/tmp/ls_output.txt";

    char command[BUFFER_SIZE + 39]; // "ls -l " + directory name + " > /tmp/ls_output.txt"
    sprintf(command, "ls -l \"%s\" > %s", buffer, temp_file);
    system(command);

    // die temporare Datei offnen
    FILE *temp_file_ptr = fopen(temp_file, "r");
    if (temp_file_ptr == NULL) {
        perror("Failed to open temporary file");
        exit(EXIT_FAILURE);
    }

    // buffer fur Zeilen und Kontor
    unsigned long long total_bytes = 0;
    char line_buffer[BUFFER_SIZE];

    // alle Zeilen reichenache lesen
    while (fgets(line_buffer, BUFFER_SIZE, temp_file_ptr) != NULL) {
        if (strncmp(line_buffer, "total", 5) == 0)
            continue;

        //  nutzlose Daten uberspringen
        strtok(line_buffer, " ");

        // line count
        strtok(NULL, " ");

        // owner and group
        strtok(NULL, " ");
        strtok(NULL, " ");

        // file size lesen
        char *size_str = strtok(NULL, " ");
        if (size_str != NULL) {
            unsigned long long file_size = strtoull(size_str, NULL, 10);
            total_bytes += file_size;
        }
    }

    // temporary file schliessen
    fclose(temp_file_ptr);

    // temporary file entfernen
    unlink(temp_file);

    // der schreib-Ende der FIFO schliessen
    close(pipe_fd);

    // der FIFO pipe in write-only offnen fur die totalBytes
    pipe_fd = open(FIFO_NAME, O_WRONLY);
    if (pipe_fd == -1) {
        perror("Failed to open FIFO pipe");
        exit(EXIT_FAILURE);
    }

    // totalBytes schreiben
    ssize_t bytes_written = write(pipe_fd, &total_bytes, sizeof(total_bytes));
    if (bytes_written == -1) {
        perror("Failed to write to FIFO pipe");
        exit(EXIT_FAILURE);
    }

    // die schreib-Ende schliessen
    close(pipe_fd);

    // FIFO entfernen
    unlink(FIFO_NAME);

    return 0;
}
