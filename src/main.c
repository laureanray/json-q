#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

#include "token.h"

int main(int argc, char *argv[]) {
  /* FILE *file; */
  /* char filename[200] = "input.json"; */
  /* file = fopen(filename, "r"); */
  /* if (file) { */
  /*   int chr; */
  /*   while ((chr = fgetc(file)) != EOF) */
  /*     fputc(chr, stdout); */
  /*   fclose(file); */
  /* } else { */
  /*   printf("File not found."); */
  /* } */
  /* printf("\n"); */
  /*  */
  /* return 0; */
    struct Token* tkn = createToken();

    tkn->Type = "Test";
    tkn->Literal = "Something";

    printf("%s\n", tkn->Literal);
}
