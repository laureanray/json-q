#include <stdlib.h>
#define LBRACE "{"
#define RBRACE "}"

typedef enum {
    LPAREN
} TokenType;

struct Token {
  TokenType type;
  char literal;
};

struct Token* newToken(TokenType tt, char ch);
