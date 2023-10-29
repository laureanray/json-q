#include "file.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char* bufferResult = loadFile("../input.json");
    struct Lexer* lexer = lexerCreate(bufferResult);
    struct Parser* parser = newParser(lexer);

    parser_next_token(parser);
    printf("%s", parser->peek_token->literal);
    while(parser->peek_token->type != END_OF_FILE) {
        parser_next_token(parser);
        printf("%s", parser->peek_token->literal);
    }

    /* while(parser->peek_token->type != END_OF_FILE) { */
    /*     parser_next_token(parser); */
    /*  */
    /*     if (parser->curr_token && parser->curr_token->literal != NULL) { */
    /*         printf("%s", parser->curr_token->literal); */
    /*     } */
    /* } */
}
