#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define IF_NOT_NULL_DESTROY_STRING(p)

Pair *parse_pair(Token **start);
Object *parse_object(Token **start);
List *parse_list(Token **start);

void destroy_list(List *list);
void destroy_object(Object *obj);

void parse_pair_into(Token **start, Pair *pair_out);

void destroy_value(ValueTyped *vt) {
  if (vt == NULL || vt->type == 0) {
    return;
  }

  switch (vt->type) {
  case STRING_VALUE:
    if (vt->val->str != NULL)
      free(vt->val->str);
    break;
  case OBJECT:
    if (vt->val->obj != NULL)
      destroy_object(vt->val->obj);
    break;
  case LIST:
    if (vt->val->list != NULL)
      destroy_list(vt->val->list);
    break;
  }

  free(vt->val);
  free(vt);
}

void destroy_list(List *list) {
  if (list == NULL) return;

  for (int i = 0; i < list->len; i++) {
    ValueTyped *elem = &list->elems[i];
    if (elem->type == STRING_VALUE && elem->val->str != NULL) {
      free(elem->val->str);
    } else if (elem->type == OBJECT && elem->val->obj != NULL) {
      destroy_object(elem->val->obj);
    } else if (elem->type == LIST && elem->val->list != NULL) {
      destroy_list(elem->val->list);
    }
    if (elem->val != NULL) {
      free(elem->val);
    }
  }

  free(list->elems);
  free(list);
}

void destroy_pair(Pair *pair) {
  if (pair == NULL) return;
  
  free(pair->key);
  if (pair->value != NULL) {
    destroy_value(pair->value);
  }
}

void destroy_object(Object *obj) {
  if (obj == NULL) return;

  for (int i = 0; i < obj->pair_count; i++) {
    Pair *pair = &obj->pairs[i];
    free(pair->key);
    if (pair->value != NULL) {
      if (pair->value->type == STRING_VALUE && pair->value->val->str != NULL) {
        free(pair->value->val->str);
      } else if (pair->value->type == OBJECT && pair->value->val->obj != NULL) {
        destroy_object(pair->value->val->obj);
      } else if (pair->value->type == LIST && pair->value->val->list != NULL) {
        destroy_list(pair->value->val->list);
      }
      if (pair->value->val != NULL) {
        free(pair->value->val);
      }
      free(pair->value);
    }
  }

  free(obj->pairs);
  free(obj);
}

ValueTyped *parse_value(Token **start) {
  if (start == NULL || *start == NULL) {
    fprintf(stderr, "start is NULL\n");
    return NULL;
  }

  Token *p = *start;
  ValueTyped *value_typed = malloc(sizeof(ValueTyped));
  if (!value_typed) {
    fprintf(stderr, "failed allocate memory for typed value");

    return NULL;
  }

  value_typed->val = malloc(sizeof(Value));
  if (!value_typed->val) {
    fprintf(stderr, "failed allocate memory for value");

    free(value_typed);

    return NULL;
  }

  value_typed->type = 0;

  value_typed->val->str = NULL;
  value_typed->val->obj = NULL;
  value_typed->val->list = NULL;

  switch (p->type) {
  case STRING: {
    char *str = calloc(p->len - 1, sizeof(char));
    if (!str) {
      fprintf(stderr, "failed allocate memory for value");

      goto fail;
    }

    memcpy(str, p->start + 1, p->len - 2);

    str[p->len - 2] = '\0';

    value_typed->type = STRING_VALUE;

    value_typed->val->str = str;

    p++;

    break;
  }

  case L_ARR_BRACE: {
    List *lst = parse_list(&p);
    if (lst == NULL) {
      fprintf(stderr, "failed parse list");

      goto fail;
    }

    value_typed->type = LIST;

    value_typed->val->list = lst;
    break;
  }

  case LBRACE: {
    Object *obj = parse_object(&p);
    if (obj == NULL) {
      fprintf(stderr, "failed parse object");

      goto fail;
    }

    value_typed->type = OBJECT;
    value_typed->val->obj = obj;
    break;
  }

  default:
    fprintf(stderr, "unknown token type");

    goto fail;
  }

  *start = p;

  return value_typed;

fail:
  destroy_value(value_typed);
  return NULL;
}

void parse_pair_into(Token **start, Pair *pair_out) {
  if (start == NULL || *start == NULL || pair_out == NULL) {
    fprintf(stderr, "invalid input to parse_pair_into\n");
    return;
  }

  Token *p = *start;

  if (p->type != STRING) {
    fprintf(stderr, "key token must be string\n");
    return;
  }

  char *key = calloc(p->len - 1, sizeof(char));  // -1 for null terminator, -2 for quotes but +1 for null, wait
  if (!key) {
    fprintf(stderr, "failed allocate memory for key\n");
    return;
  }
  memcpy(key, p->start + 1, p->len - 2);
  key[p->len - 2] = '\0';

  p++;

  if (p->type != COLON) {
    fprintf(stderr, "token after key must be colon\n");
    free(key);
    return;
  }

  p++;

  pair_out->key = key;

  ValueTyped *value_typed = parse_value(&p);
  if (value_typed == NULL) {
    fprintf(stderr, "failed parse value\n");
    free(pair_out->key);
    pair_out->key = NULL;
    return;
  }

  pair_out->value = value_typed;
  *start = p;
}

int count_array_elements(Token **start) {
  Token *p = *start;

  if (!p || p->type != L_ARR_BRACE) {
    fprintf(stderr, "expexted token");
    return -1;
  }

  int depth = 1;
  int count = 0;
  int after_comma = 0;

  p++;

  while (p && depth > 0) {
    if (p->type == L_ARR_BRACE || p->type == LBRACE) {
      if (depth == 1) {
        count++;
      }
      depth++;
    } else if (p->type == R_ARR_BRACE || p->type == RBRACE) {
      depth--;
    } else if (depth == 1 && p->type != COMMA) {
      count++;
    }

    p++;
  }

  if (depth != 0) {
    fprintf(stderr, "Error: unclosed list or array\n");
    return -1;
  }

  return count;
}

List *parse_list(Token **start) {
  Token *p = *start;

  if (!p || p->type != L_ARR_BRACE) {
    fprintf(stderr, "expected opening bracket for list");
    return NULL;
  }

  int len = count_array_elements(&p);
  if (len < 0) {
    fprintf(stderr, "failed count array elements");

    return NULL;
  }

  List *list = malloc(sizeof(List));
  if (!list) {
    fprintf(stderr, "failed allocate memory for list");

    return NULL;
  }

  list->elems = calloc(len, sizeof(ValueTyped));
  if (!list->elems) {
    fprintf(stderr, "failed allocate memory for elements");

    free(list);

    return NULL;
  }

  int index = 0;
  list->len = len;

  p = *start + 1;

  while (p->type != R_ARR_BRACE) {
    if (p->type == COMMA) {
      p++;
      continue;
    }

    ValueTyped *value = parse_value(&p);
    if (value == NULL) {
      fprintf(stderr, "failed parse value from list\n");
      destroy_list(list);
      return NULL;
    }

    list->elems[index] = *value;
    free(value);
    index++;
  }

  p++; // skip closing bracket
  *start = p;

  return list;
}

int count_pairs_in_object(Token **start) {
  Token *p = *start;

  if (!p || p->type != LBRACE) {
    fprintf(stderr, "expexted token");
    return -1;
  }

  int depth = 1;
  int count = 0;
  int after_key = 0;

  p++;

  while (depth != 0) {
    if (p->type == STRING) {
      if (depth == 1 && after_key == 0) {
        // This is a key
        after_key = 1;
        count++;
      } else if (depth == 1 && after_key == 1) {
        // This is a value after key:value pair, reset
        after_key = 0;
      }
    } else if (p->type == COLON) {
      // Colon after key, still in pair
      if (depth == 1) after_key = 1;
    } else if (p->type == COMMA) {
      // Comma separates pairs
      if (depth == 1) after_key = 0;
    }

    if (p->type == L_ARR_BRACE || p->type == LBRACE) {
      depth++;
    }

    if (p->type == R_ARR_BRACE || p->type == RBRACE) {
      depth--;
    }

    p++;
  }

  return count;
}

Object *parse_object(Token **start) {
  Token *p = *start;

  if (!p || p->type != LBRACE) {
    fprintf(stderr, "expected opening brace for object");
    return NULL;
  }

  Object *obj = malloc(sizeof(Object));
  if (!obj) {
    fprintf(stderr, "failed allocate memory for object");
    return NULL;
  }

  int pair_count = count_pairs_in_object(&p);
  if (pair_count < 0) {
    fprintf(stderr, "failed count pairs");

    free(obj);

    return NULL;
  }

  Pair *pairs = calloc(pair_count, sizeof(Pair));
  if (!pairs) {
    fprintf(stderr, "failed allocate memory for pairs");

    free(obj);

    return NULL;
  }

  obj->pair_count = pair_count;
  obj->pairs = pairs;

  if (pair_count == 0) {
    p = *start + 1;
    if (p->type != RBRACE) {
      fprintf(stderr, "expected closing brace for empty object");
      free(obj->pairs);
      free(obj);
      return NULL;
    }
    p++;
    *start = p;
    return obj;
  }

  p = *start + 1;
  int pair_index = 0;

  while (p->type != RBRACE && pair_index < pair_count) {
    if (p->type == COMMA) {
      p++;
      continue;
    }

    parse_pair_into(&p, &pairs[pair_index]);
    if (pairs[pair_index].key == NULL) {
      fprintf(stderr, "failed parse pair from object");
      destroy_object(obj);
      return NULL;
    }

    pair_index++;
  }

  if (p->type != RBRACE) {
    fprintf(stderr, "expected closing brace for object\n");
    free(obj->pairs);
    free(obj);
    return NULL;
  }

  p++; // skip closing brace
  *start = p;

  return obj;
}

Object *tokens_to_ast(Token *tokens) {
  Token *p = tokens;

  Object *ast = parse_object(&p);
  if (ast == NULL) {
    fprintf(stderr, "failed parse json data to ast");

    return NULL;
  }

  return ast;
}
