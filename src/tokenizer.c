#include "tokenizer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int get_token_numbers(const char *data) {
  if (data == NULL) {
    return 0;
  }

  const char *p = data;

  int number = 0;

  bool in_str = true;
  while (*p != '\0') {
    if (in_str) {
      if (*p == '"') {
        number++;

        in_str = false;
      }
    } else if (*p == '{' | *p == '}' | *p == ':' | *p == ',' | *p == '[' |
               *p == ']') {
      number++;
    } else if (*p == '"') {
      in_str = true;
    }

    p++;
  }

  return number;
}

Token get_token_from_string(const char **start) {
  Token tkn;

  const char *p = *start + 1;

  int len = 0;

  while (*p != '"') {
    len++;
    p++;
  }

  tkn.type = STRING;
  tkn.start = *start;
  tkn.len = len + 2;

  *start = p + 2;

  return tkn;
}

Token get_next_token(const char **pos, int *opened_braces) {
  const char *p = *pos;

  while (*p && isspace(*p))
    p++;

  if (!*p) {
    Token tkn;

    tkn.type = EOF;

    return tkn;
  }

  Token tkn;

  switch (*p) {
  case '{':
    tkn.type = LBRACE;
    tkn.start = p;
    tkn.len = 1;

    (*opened_braces)++;
    (*pos)++;

    return tkn;
  case '}':
    tkn.type = RBRACE;
    tkn.start = p;
    tkn.len = 1;

    (*opened_braces)--;
    (*pos)++;

    return tkn;
  case ',':
    tkn.type = COMMA;
    tkn.start = p;
    tkn.len = 1;

    (*pos)++;

    return tkn;
  case ':':
    tkn.type = COLON;
    tkn.start = p;
    tkn.len = 1;

    (*pos)++;

    return tkn;

  case '"':
    tkn = get_token_from_string(&p);
    *pos = p;
    return tkn;
  default:
    tkn.type = UNKNOWN;

    return tkn;
  }
}

Token *tokenize(const char *data) {
  Token *start = calloc(MAX_TOKEN_NUMBER, sizeof(Token));
  Token *position = start;
  Token tkn;

  const char *p = data;

  int opened_braces = 0;

  do {
    Token tkn = get_next_token(&p, &opened_braces);

    printf("new token with len: %zu\n", tkn.len);

    if (position - start >= MAX_TOKEN_NUMBER - 1) {
      fprintf(stderr, "Too many tokens (max %d)\n", MAX_TOKEN_NUMBER - 1);
      free(start);
      return NULL;
    }

    *position = tkn;
    position++;

    if (tkn.type == EOF_TOKEN)
      break;

    if (tkn.type == UNKNOWN) {
      fprintf(stderr, "Unknown token near position %zu\n", (size_t)(p - data));
      free(start);
      return NULL;
    }
  } while (opened_braces != 0);

  return start;
}
