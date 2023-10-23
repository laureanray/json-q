#include "lexer.h"
#include "token.h"
struct Parser {
    Lexer* lexer;
    Token* curr_token;
    Token* peek_token;
    char* errors[];
};

