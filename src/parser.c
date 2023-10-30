#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

struct Parser *newParser(struct Lexer *lexer) {
    struct Parser *p = malloc(sizeof(struct Parser));
    p->lexer = lexer;
    return p;
}

static struct JsonMember* _parser_parse_json_member(struct Parser* parser);
static struct JsonMember* _parser_parse_entry(struct Parser* parser);
static struct JsonValue* _parser_parse_json_value(struct Parser* parse);
static int _parser_expect_peek(struct Parser* parser, TokenType tt);
static int _parser_peek_token_is(struct Parser* parser, TokenType tt);
static void _parser_next_token(struct Parser* parser);

static char* _parser_peek_error(struct Parser* parser, TokenType tt);

static void _parser_next_token(struct Parser *parser) {
    parser->curr_token = parser->peek_token;
    parser->peek_token = lexerNextToken(parser->lexer);
}

struct JsonObject *parseJson(struct Parser *parser) {
    _parser_next_token(parser);
    struct JsonObject* json_object = malloc(sizeof(struct JsonObject));
    struct JsonMember* members = malloc(sizeof(struct JsonMember));

    // init
    struct JsonMember* head = malloc(sizeof(struct JsonMember));

    if (parser->curr_token != NULL) {
        head->key = parser->curr_token->literal;
        head->value  = _parser_parse_json_value(parser);

        json_object->members = head;
    } else {
        perror("Current token is null!");
    }

    while (parser->peek_token->type != END_OF_FILE) {
        struct JsonMember* temp = json_object->members->next;
        struct JsonMember* json_member = malloc(sizeof(struct JsonMember));
        json_member->key = parser->curr_token->literal;
        json_member->value = _parser_parse_json_value(parser);
        if (temp == NULL) {
            json_object->members->next = json_member;
        } else {
            temp->next = json_member;
        }
    }

    return json_object;
}

static struct JsonMember* _parser_parse_entry(struct Parser *parser) {
    switch (parser->curr_token->type) {
        case STRING:
            return _parser_parse_json_member(parser);
        default:
            perror("invalid token");
    }

    return NULL;
}

static struct JsonValue* _parser_parse_json_value(struct Parser* parser) {
    struct JsonValue* json_val = malloc(sizeof(struct JsonValue));

    switch(parser->curr_token->type) {
        // TOKEN TYPE
        case STRING:
            json_val->type = JSON_STRING;
            json_val->string_value = parser->curr_token->literal;
            break;
        default:
            perror("Invalid token type unable to parse json value");
    }

    return json_val;
}

static struct JsonMember* _parser_parse_json_member(struct Parser *parser) {
    struct JsonMember* jm = malloc(sizeof(struct JsonMember));
    return jm;
}

static int _parser_peek_token_is(struct Parser* parser, TokenType tt) {
    if (parser->curr_token->type == tt) {
        return 1;
    } else {
        return 0;
    }
}

static int _parser_expect_peek(struct Parser* parser, TokenType tt) {
    if (_parser_peek_token_is(parser, tt) == 1) {
        _parser_next_token(parser);
        return 1;
    } else {
        _parser_expect_peek(parser, tt);
        return 0;
    }
}




static char* _parser_peek_error(struct Parser* parser, TokenType tt) {
    char* err;
    sprintf("expected token to be %d got %d instead", err, tt, parser->peek_token->type);
    return err;
}
