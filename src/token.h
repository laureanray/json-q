#include <stdlib.h>
#define LBRACE "{"
#define RBRACE "}"

struct Token {
    char* Type;
    char* Literal;
};


struct Token* createToken();
