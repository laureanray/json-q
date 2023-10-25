#include "parser.h"
#include <stdlib.h>
struct Parser* newParser(struct Lexer* lexer) {
    struct Parser* p = malloc(sizeof(struct Parser));
    p->lexer = lexer;
    return p;
}
