#include "lexer.h"
#include "token.h"

struct Parser {
  struct Lexer *lexer;
  struct Token *curr_token;
  struct Token *peek_token;
  char *errors[];
};

struct Parser* newParser(struct Lexer *lexer);
