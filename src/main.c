#include "file.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char* bufferResult = loadFile("../input.json");
    printf("%zu\n", strlen(bufferResult));
    printf("%s\n", bufferResult);
    
    struct Lexer* lexer = lexerCreate(bufferResult);
    struct Parser* parser = newParser(lexer);
}
