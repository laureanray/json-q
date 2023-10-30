#include "lexer.h"
#include "token.h"
#include <stdlib.h>

struct Parser {
  struct Lexer *lexer;
  struct Token *curr_token;
  struct Token *peek_token;
  char *errors[];
};

struct JsonObject {
    int num_members; // key?
    struct JsonMember* members;
};

struct JsonMember {
    char* key;
    struct JsonValue* value;
    struct JsonMember* next;
};

struct JsonArray {
    int num_elements;
    struct JsonValue* elements;
};

typedef enum {
    JSON_STRING,
    JSON_NUMBER,
    JSON_BOOLEAN,
    JSON_NULL,
    JSON_OJBECT,
    JSON_ARRAY
} JsonValueType;

struct JsonValue {
    JsonValueType type;
    union {
        char* string_value;
        double number_value;
        int boolean_value;
        struct JsonObject* object;
        struct JsonArray* array;
    };
};

struct Parser* new_parser(struct Lexer *lexer);
struct JsonObject* parse_json(struct Parser *parser);

void parser_next_token(struct Parser* parser);
