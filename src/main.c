#include "file.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char* bufferResult = loadFile("../input.json");
    struct Lexer* lexer = lexerCreate(bufferResult);
    struct Parser* parser = newParser(lexer);

    while(lexer->ch != '\0') {
        struct Token* tok = lexerNextToken(lexer);
        if (tok && tok->literal != NULL) {
            printf("%s", tok->literal);
        }
    }
}
