#include "file.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("%s", "loading file...");
    char* buffer; 
    int loadFileResult = loadFile("input.json", buffer);
    printf("%s", buffer);
}
