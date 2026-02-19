#include "tokenizer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

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
  tkn.len = len;

  *start = p + 1;

  return tkn;
}

Token get_next_token(const char **pos) {
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

    (*pos)++;

    return tkn;
  case '}':
    tkn.type = RBRACE;
    tkn.start = p;
    tkn.len = 1;

    (*pos)++;

    return tkn;
  case ',':
    tkn.type = COMMA;
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

  do {
    tkn = get_next_token(&p);
    if (tkn.type == EOF) {
      break;
    }
    if (tkn.type == UNKNOWN) {
      perror("unknown token type");
      break;
    }

    if (position - start >= MAX_TOKEN_NUMBER - 1) {
      perror("overload");
      break;
    }

  } while (tkn.type != EOF);

  return start;
}
