#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_substring(char* source, size_t start, size_t length) {
    char* result = (char*) malloc(length + 1);

    if (result == NULL) {
        perror("Failure to allocate memory for substring");
        exit(EXIT_FAILURE);
    }

    strncpy(result, source + start, length);

    result[length] = '\0';

    return result;
}
