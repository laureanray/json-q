#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

struct Parser *newParser(struct Lexer *lexer) {
  struct Parser *p = malloc(sizeof(struct Parser));
  p->lexer = lexer;
  return p;
}

static struct JsonMember* parse_json_member(struct Parser *parser);
static int expect_peek(struct Parser* parser, TokenType* tt);

void parser_next_token(struct Parser *parser) {
  parser->curr_token = parser->peek_token;
  parser->peek_token = lexerNextToken(parser->lexer);
}

struct JsonObject *parseJson(struct Parser *parser) {
  struct JsonObject *jo = malloc(sizeof(struct JsonObject));
  // init
  struct JsonMember *members = malloc(sizeof(struct JsonMember));
  // TEST
  jo->num_members = 1000;
  jo->members = malloc(jo->num_members * (sizeof(struct JsonMember)));

  int counter = 0;

  while (parser->peek_token->type != END_OF_FILE) {
    jo->members[counter].key = parser->curr_token->literal;

    counter++;
  }

  return jo;
}

struct JsonMember *parse_json(struct Parser *parser) {
  switch (parser->curr_token->type) {
  case STRING:
    return parse_json_member(parser);
  default:
    perror("invalid token");
  }
}

static struct JsonMember* parse_json_member(struct Parser *parser) {
    struct JsonMember* jm = malloc(sizeof(struct JsonMember));
    

    return jm;
}
