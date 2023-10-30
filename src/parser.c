#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

struct Parser* new_parser(struct Lexer *lexer) {
    struct Parser *p = malloc(sizeof(struct Parser));
    p->lexer = lexer;
    return p;
}

static struct JsonMember* _parse_json_string(struct Parser* parser, struct JsonMember* json_member);
static struct JsonMember* _parse_entry(struct Parser* parser, struct JsonMember* json_member);
static struct JsonValue* _parse_json_value(struct Parser* parse);
static int _expect_peek(struct Parser* parser, TokenType tt);
static int _peek_token_is(struct Parser* parser, TokenType tt);
static void _next_token(struct Parser* parser);
static char* _peek_error(struct Parser* parser, TokenType tt);

static void _next_token(struct Parser *parser) {
    parser->curr_token = parser->peek_token;
    parser->peek_token = lexerNextToken(parser->lexer);
}

struct JsonObject* parse_json(struct Parser *parser) {
    _next_token(parser);
    _next_token(parser);
    struct JsonObject* json_object = malloc(sizeof(struct JsonObject));
    struct JsonMember* members = malloc(sizeof(struct JsonMember));

    // init
    struct JsonMember* head = NULL;

    if (parser->curr_token != NULL) {
        json_object->members = head;
    } else {
        perror("Current token is null!");
    }

    while (parser->peek_token->type != END_OF_FILE) {
        if (head == NULL) {
            head = malloc(sizeof(struct JsonMember));
            json_object->members = head;
        }
        struct JsonMember* temp = json_object->members->next;
        struct JsonMember* json_member = malloc(sizeof(struct JsonMember));
        _parse_entry(parser, json_member);
        if (temp == NULL) {
            json_object->members->next = json_member;
        } else {
            temp->next = json_member;
        }

        _next_token(parser);
    }

    return json_object;
}

static struct JsonMember* _parse_entry(struct Parser *parser, struct JsonMember* json_member) {
    switch (parser->curr_token->type) {
        case STRING:
            return _parse_json_string(parser, json_member);
        default:
            perror("invalid token");
    }

    return NULL;
}

static struct JsonMember* _parse_json_string(struct Parser* parser, struct JsonMember* json_member) {
    json_member->key = parser->curr_token->literal;
    json_member->value = _parse_json_value(parser);
    return json_member;
}

static struct JsonValue* _parse_json_value(struct Parser* parser) {
    struct JsonValue* json_val = malloc(sizeof(struct JsonValue));

    switch(parser->curr_token->type) {
        // TOKEN TYPE
        case STRING:
            json_val->type = JSON_STRING;
            // THIS COULD BE WRONG?
            json_val->string_value = parser->curr_token->literal;
            break;
        default:
            perror("Invalid token type unable to parse json value");
    }

    return json_val;
}

static int _peek_token_is(struct Parser* parser, TokenType tt) {
    if (parser->curr_token->type == tt) {
        return 1;
    } else {
        return 0;
    }
}

static int _expect_peek(struct Parser* parser, TokenType tt) {
    if (_peek_token_is(parser, tt) == 1) {
        _next_token(parser);
        return 1;
    } else {
        _expect_peek(parser, tt);
        return 0;
    }
}




static char* _peek_error(struct Parser* parser, TokenType tt) {
    char* err;
    sprintf("expected token to be %d got %d instead", err, tt, parser->peek_token->type);
    return err;
}
