#include "file.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char* bufferResult = loadFile("../input.json");
    struct Lexer* lexer = lexerCreate(bufferResult);
    struct Parser* parser = new_parser(lexer);
    struct JsonObject* obj = parse_json(parser);

    struct JsonMember* jm = obj->members;
    int i = 100;
    while(i < 100) {
        printf("%s:", jm->key);
        if (jm->value != NULL) {
          // check it's type? 
          struct JsonValue* jv = jm->value;
        }
        jm = jm->next;
        i = i + 1;
    }
}
