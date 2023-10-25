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

char* char_to_string(char c) {
    char* str = (char*) malloc(2 * sizeof(char));
    if (str == NULL) {
        perror("Failre to allocate memory for char_to_string");
        exit(EXIT_FAILURE);
    }

    str[0] = c;
    str[1] = '\0';

    return str;
}
