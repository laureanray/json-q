#include "lexer.h"
#include <stdlib.h>
#include <string.h>

static void _lexer_read_char(struct Lexer* lexer);

struct Lexer* lexerCreate(char* input) {
    int len = sizeof(struct Lexer);
    struct Lexer* lexer = malloc(len);
    memset(lexer, 0, len);

    lexer->input = input;
    lexer->input_length = strlen(input);
    lexer->read_position = 0;
    lexer->position = 0;

    _lexer_read_char(lexer);

    return lexer;
}


static void _lexer_read_char(struct Lexer* lexer) {
    if (lexer->read_position >= lexer->input_length) {
        lexer->ch = 0;
    } else {
        lexer->ch = lexer->input[lexer->read_position];
    }

    lexer->position = lexer->read_position;
    lexer->read_position = lexer->read_position + 1;
}
