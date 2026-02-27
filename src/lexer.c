#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Pair *parse_pair(Token **start);
Object *parse_object(Token **start);

Pair *parse_pair(Token **start) {
  Token *p = *start;

  Pair *pair = malloc(sizeof(Pair)); 
  if (!pair) {
    fprintf(stderr, "failed allocate memory for pair");

    return NULL;
  }

  Token tkn_key = *p;

  char *key = calloc(tkn_key.len + 1, sizeof(char));
  if (!key) {
    fprintf(stderr, "failed allocate memory for key");

    return NULL;
  }

  strncpy((char*)tkn_key.start, key, tkn_key.len);

  *(key+tkn_key.len+1) = '\0';

  Token tkn_value = *(p+2);

  if (tkn_value.type == LBRACE) {
    pair->type = OBJECT;

    Token *pos = p+2;

    Object *obj = parse_object(&pos);

    p = pos;

    pair->value.obj = obj;
  } else if (tkn_value.type == STRING) {
    pair->type = VALUE;

    char *value = calloc(tkn_value.len+1, sizeof(char));
    if (!value) {
      fprintf(stderr, "failed allocate memory for value");

      return NULL;
    }

    strncpy((char*)tkn_value.start, value, tkn_value.len);

    *(value+tkn_value.len+1) = '\0'; 

    pair->value.value = value;
  } else {
    fprintf(stderr, "token is not lbrace or string");

    return NULL;
  }

  return pair;
}

Object *parse_object(Token **start) {}

Object *tokens_to_ast(Token *tokens) {}
