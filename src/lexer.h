#include <stddef.h>

struct Lexer {
    char* input;
    size_t input_length;
    size_t position;
    size_t read_position;
    char ch;
};

struct Lexer* lexerCreate(char *input);
struct Token* lexerNextToken(struct Lexer* lexer);
