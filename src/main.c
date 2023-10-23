#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char* bufferResult = loadFile("../input.json");
    printf("%zu\n", strlen(bufferResult));
    printf("%s\n", bufferResult);
}
