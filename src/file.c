#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int loadFile(char* filename, char* buffer) {
    FILE *file = fopen(filename, "rb");
    long file_size;

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    buffer = (char *)malloc(file_size);

    if (buffer == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return 1;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);

    printf("bytes read: %zu \n", bytes_read);

    if (bytes_read != file_size) {
        perror("Error reading file");
        // De-allocate memory
        free(buffer);
        return 1;
    }

    // fclose(file);

    return 0;
}
