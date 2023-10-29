#include "parser.h"
#include <stdlib.h>

struct Parser* newParser(struct Lexer* lexer) {
    struct Parser* p = malloc(sizeof(struct Parser));
    p->lexer = lexer;
    return p;
}


void parser_next_token(struct Parser* parser) {
    parser->curr_token = parser->peek_token;
    parser->peek_token = lexerNextToken(parser->lexer);
}
