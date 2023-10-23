#include "file.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("%s", "loading file...");
    char* buffer; 
    int loadFileResult = loadFile("input.json", buffer);
    printf("%s", buffer);
    /* free(buffer); */
}
