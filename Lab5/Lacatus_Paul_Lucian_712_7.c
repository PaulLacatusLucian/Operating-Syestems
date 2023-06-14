#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fileptr;
    char *buffer;
    long filelen;

    if (argc != 1) {
        printf("Bitte geben Sie den Dateinamen als Argument ein.\n");
        return 1;
    }

    fileptr = fopen(argv[1], "rb");
    if (fileptr == NULL) {
        printf("Die Datei konnte nicht geoffnet werden.\n");
        return 1;
    }

    fseek(fileptr, 0, SEEK_END);
    filelen = ftell(fileptr);
    rewind(fileptr);

    buffer = (char *)malloc((filelen+1)*sizeof(char));
    if (buffer == NULL) {
        printf("Speicher konnte nicht allokiert werden.\n");
        return 1;
    }

    fread(buffer, filelen, 1, fileptr);
    fclose(fileptr);

    for (int i = 0; i < filelen/2; i++) {
        char temp = buffer[i];
        buffer[i] = buffer[filelen-1-i];
        buffer[filelen-1-i] = temp;
    }

    fileptr = fopen(argv[1], "wb");
    if (fileptr == NULL) {
        printf("Die Datei konnte nicht geoffnet werden.\n");
        return 1;
    }

    fwrite(buffer, filelen, 1, fileptr);
    fclose(fileptr);

    free(buffer);

    return 0;
}
