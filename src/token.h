#include <stdlib.h>

typedef enum {
    LBRACKET,
    RBRACKET,
    LBRACE,
    RBRACE,
    COLON,
    COMMA,
    STRING,
    NUMBER,
    TRUE,
    FALSE,
    NIL, // NULL
} TokenType;

struct Token {
  TokenType type;
  char* literal;
};

struct Token* newToken(TokenType tt, char ch);
