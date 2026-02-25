#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

Pair *parse_pair(Token **start);
Object *parse_object(Token **start);

Pair *parse_pair(Token **start) {
  Token *p = *start;

  Token tkn = *p;

  char *key = calloc(tkn.len + 1, sizeof(char));
  if (!key) {
    fprintf(stderr, "failed allocate memory for key");

    return NULL;
  }
}

Object *parse_object(Token **start) {}

Object *tokens_to_ast(Token *tokens) {}
