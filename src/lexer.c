#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Pair *parse_pair(Token **start);
Object *parse_object(Token **start);
List *parse_list(Token **start);

typedef struct {
  Value *val;
  enum ValueType type;
} ValueTyped;


ValueTyped *parse_value(Token **start) {
  Token *p = *start;
  ValueTyped *value_typed = malloc(sizeof(ValueTyped));

  switch (p->type) {
  case STRING: {
    char *value = calloc(p->len + 1, sizeof(char));
    if (!value) {
      fprintf(stderr, "failed allocate memory for value");

      return NULL;
    }

    memcpy(value, p->start, p->len);

    value[p->len] = '\0';

    value_typed->type = STRING_VALUE;

    value_typed->val->str = value;
  }

  case L_ARR_BRACE: {
    List *lst = parse_list(&p);
    if (lst == NULL) {
      fprintf(stderr, "failed parse list");

      return NULL;
    }

    value_typed->type = LIST;

    value_typed->val->list = lst;
  }

  case LBRACE: {
    Object *obj = parse_object(&p);
    if (obj == NULL) {
      fprintf(stderr, "failed parse object");

      return NULL;
    }

    value_typed->type = OBJECT;
    value_typed->val->obj = obj;
  }

  default:
    fprintf(stderr, "unknown token type");
  }

  *start = p;

  return value_typed;
}

Pair *parse_pair(Token **start) {
  Token *p = *start;

  Pair *pair = malloc(sizeof(Pair));
  if (!pair) {
    fprintf(stderr, "failed allocate memory for pair");

    return NULL;
  }

  Token *tkn_key = p;
  if (tkn_key->type != STRING) {
    fprintf(stderr, "key token must be string");

    return NULL;
  }

  char *key = calloc(tkn_key->len + 1, sizeof(char));
  if (!key) {
    fprintf(stderr, "failed allocate memory for key");

    return NULL;
  }

  memcpy(key, tkn_key->start, tkn_key->len);

  key[tkn_key->len] = '\0';

  p++;

  Token *tkn_colon = p;

  if (tkn_colon->type != COLON) {
    fprintf(stderr, "token after key must be colon");

    free(key);

    return NULL;
  }

  p++;

  Token *tkn_value = p;

  ValueTyped *value_typed = parse_value(&p);

  pair->type = value_typed->type;
  switch (value_typed->type) {
  case STRING_VALUE:
    pair->value.str = value_typed->val->str;
  case OBJECT:
    pair->value.obj = value_typed->val->obj;
  case LIST:
    pair->value.list = value_typed->val->list;
  }

  *start = p;

  return pair;
}

int to_next_brace(Token **start, enum TokenType brace) {
  Token *p = *start;

  enum TokenType brace_to_find;

  switch (brace) {
  case L_ARR_BRACE:
    brace_to_find = R_ARR_BRACE;
  case LBRACE:
    brace_to_find = RBRACE;
  default:
    fprintf(stderr, "unexpected brace type");
    
    return 1;
  }

  p++;

  int opened_branches = 1;

  while (opened_branches != 0) {
    if (p->type == brace_to_find) {
      if (opened_branches == 1) {
        break;
      } else {
        opened_branches--;
      }
    }

    if (p->type == brace) {
      opened_branches++;
    }
    
    p++;
  }

  *start = p;

  return 0;
}

int get_elements_number(Token **start) {
  Token *p = *start;

  if (p->type != L_ARR_BRACE) {
    fprintf(stderr, "start token of a list must be with L_ARR_BRACE type");

    return -1;
  } 

  p++;

  if (p->type == L_ARR_BRACE || p->type == LBRACE) {
    
  }
}

List *parse_list(Token **start) {
  Token *p = *start;

  int len = 1;

  int opened_branches = 1;

  p++;

  while (opened_branches != 0) {
    if (p->type == L_ARR_BRACE) {
      opened_branches++;
    } else if (p->type == R_ARR_BRACE) {
      opened_branches--;
    }
    p++;
    len++;
  }

  List *list = malloc(sizeof(List));
  if (!list) {
    fprintf(stderr, "failed allocate memory for list");

    return NULL;
  }

  list->elems = calloc(len, sizeof(Value));
  if (!list->elems) {
    fprintf(stderr, "failed allocate memory for elements");

    free(list);

    return NULL;
  }

  list->len = len;

  p = *start + 1;


  return list;
}

Object *parse_object(Token **start) {}

Object *tokens_to_ast(Token *tokens) {}
