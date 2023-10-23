#include "token.h"

struct Token* createToken() {
    struct Token *ptr;
    ptr = (struct Token*) malloc(sizeof(struct Token));
    return ptr;
}
