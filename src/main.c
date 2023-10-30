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
    if (obj->members != NULL) {
        printf("%s\n", obj->members->key);
    }
}
