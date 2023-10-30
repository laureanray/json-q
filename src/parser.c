#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

struct Parser* new_parser(struct Lexer *lexer) {
    struct Parser *p = malloc(sizeof(struct Parser));
    p->lexer = lexer;
    return p;
}

static int _parse_json_string(struct Parser* parser, struct JsonMember* json_member);
static int _parse_entry(struct Parser* parser, struct JsonMember* json_member);
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
        struct JsonMember* json_member;
        if (head == NULL) {
            head = malloc(sizeof(struct JsonMember));
            json_object->members = head;
        } else {
            json_member = malloc(sizeof(struct JsonMember));
        }

        struct JsonMember* head_next = json_object->members->next;

        int parse_result;
        if (head->key == NULL) {
            parse_result = _parse_entry(parser, head);
        } else {
            parse_result = _parse_entry(parser, json_member);
        }

        if (head_next == NULL) {
            if (parse_result == 1) {
                json_object->members->next = json_member;
            }
        } else {
            head_next->next = json_member;
        }

        _next_token(parser);
    }

    return json_object;
}

static int _parse_entry(struct Parser *parser, struct JsonMember* json_member) {
    switch (parser->curr_token->type) {
        case STRING:
            return _parse_json_string(parser, json_member);
        default:
            return 0;
            // perror("invalid token");
    }

    return 0;
}

static int _parse_json_string(struct Parser* parser, struct JsonMember* json_member) {
    json_member->key = parser->curr_token->literal;

    if (_expect_peek(parser, COLON) == 0) {
        perror("Invalid tok expected colon");
    }

    // TODO: Make this array, number and bool!
    if (_expect_peek(parser, STRING) == 0) {
        perror("Invalid tok expected string");
    }

    struct JsonValue* jv = _parse_json_value(parser);
    if (jv != NULL) {
        json_member->value = jv;
        return 1;
    }
    return 0;
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
    if (parser->peek_token->type == tt) {
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
