#include "util.h"
#include "token.h"

struct Token* newToken(TokenType tt, char ch) {
    struct Token *t = malloc(sizeof(struct Token));
    t->literal = char_to_string(ch);
    t->type = tt;
    return t;
}
