#include "token.h"

struct Token* newToken(TokenType tt, char ch) {
    struct Token *t = malloc(sizeof(struct Token));
    t->literal = ch;
    t->type = tt;
    return t;
}
