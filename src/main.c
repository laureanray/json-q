#include "file.h"
#include "lexer.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char* bufferResult = loadFile("../input.json");
    printf("%zu\n", strlen(bufferResult));
    printf("%s\n", bufferResult);
    
    struct Lexer* lexer = lexerCreate(bufferResult);

    int len = strlen(bufferResult);

    // Sanity check. 
    /* for (int i = 0; i < len; i++) { */
    /*     struct Token* tok = lexerNextToken(lexer); */
    /*     printf("%c", (char) tok->literal); */
    /* } */
}
