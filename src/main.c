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
    printf("%s\n", jm->key);
    while(jm->next != NULL) {
        printf("%s\n", jm->key);
        jm = jm->next;
    }
}
