#include "token.h"
#include "util.h"
#include "lexer.h"
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void _lexer_read_char(struct Lexer* lexer);
static void _lexer_skip_whitespace(struct Lexer* lexer);
static char _lexer_peek_char(struct Lexer* lexer);
static char* _lexer_read_string(struct Lexer* lexer);

struct Lexer* lexerCreate(char* input) {
    int len = sizeof(struct Lexer); struct Lexer* lexer = malloc(len);
    memset(lexer, 0, len);

    lexer->input = input;
    lexer->input_length = strlen(input);
    lexer->read_position = 0;
    lexer->position = 0;

    _lexer_read_char(lexer);

    return lexer;
}

struct Token* lexerNextToken(struct Lexer* lexer) {
    struct Token* tok;
    _lexer_skip_whitespace(lexer);


    switch(lexer->ch) {
        case '{':
            tok = newToken(LBRACE, lexer->ch);
            break;
        case '}':
            tok = newToken(RBRACE, lexer->ch);
            break;
        case '[':
            tok = newToken(LBRACKET, lexer->ch);
            break;
        case ']':
            tok = newToken(RBRACKET, lexer->ch);
            break;
        case ':':
            tok = newToken(COLON, lexer->ch);
            break;
        case ',':
            tok = newToken(COMMA, lexer->ch);
            break;
        case '"':
            tok = newToken(STRING, NULL);
            tok->type = STRING;
            tok->literal = _lexer_read_string(lexer);
            break;
        case '\0':
            tok = newToken(END_OF_FILE, lexer->ch);
            break;
        default:
            tok = newToken(ILLEGAL, lexer->ch);
    }

    _lexer_read_char(lexer);
    return tok;
}


static void _lexer_read_char(struct Lexer* lexer) {
    if (lexer->read_position >= lexer->input_length) {
        lexer->ch = '\0';
    } else {
        lexer->ch = lexer->input[lexer->read_position];
    }

    lexer->position = lexer->read_position;
    lexer->read_position = lexer->read_position + 1;
}

static void _lexer_skip_whitespace(struct Lexer* lexer) {
    while(lexer->ch == ' ' || lexer->ch == '\t' || lexer->ch == '\n' || lexer->ch == '\r') {
        _lexer_read_char(lexer);
    }
}

static char _lexer_peek_char(struct Lexer* lexer) {
    if(lexer->read_position >= lexer->input_length) {
        return 0;
    } else {
        return lexer->input[lexer->read_position];
    }
}


static char* _lexer_read_string(struct Lexer* lexer) {
    size_t position = lexer->position + 1;

    for(;;){
        _lexer_read_char(lexer);

        if (lexer->ch == '"' || lexer->ch == 0) {
            break;
        }
    }
    return get_substring(lexer->input, position, lexer->position - position);
}
