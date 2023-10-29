#include "parser.h"
#include <stdlib.h>

static void _parser_next_token(struct Parser* parser);

struct Parser* newParser(struct Lexer* lexer) {
    struct Parser* p = malloc(sizeof(struct Parser));
    p->lexer = lexer;
    return p;
}


static void _lexer_next_token(struct Parser* parser) {
    parser->curr_token = parser->peek_token;
    parser->peek_token = lexerNextToken(parser->lexer);
}
