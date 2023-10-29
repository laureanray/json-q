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

static void _lexer_read_char(struct Lexer* lexer);
static void _lexer_skip_whitespace(struct Lexer* lexer);
static char _lexer_peek_char(struct Lexer* lexer);
static char* _lexer_read_string(struct Lexer* lexer);
