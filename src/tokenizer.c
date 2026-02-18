#include "tokenizer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **create_tokens(int token_number) {
  char **tokens = (char **)calloc(token_number, sizeof(char *));
  if (!tokens) {
    perror("failed calloc memory");
  }

  for (int i = 0; i < token_number; i++) {
    tokens[i] = (char *)calloc(TOKEN_MAX_SIZE, sizeof(char));
    if (!tokens[i]) {
      perror("failed calloc memory");
    }
  }

  return tokens;
}

void parse_tokens(const char *data, char **tokens) {
  const char *ptr = data;

  int tokens_index = 0;
  int current_token_index = 0;

  enum State state = NORMAL;

  char token_buf[TOKEN_MAX_SIZE];
  int buf_index = 0;

  while (*ptr != '\0') {
    if (state == IN_STRING) {
      if (*ptr == '"') {
        strncpy(token_buf, tokens[tokens_index], buf_index);

        tokens_index++;
        buf_index = 0;

        state = NORMAL;
      } else {
        token_buf[buf_index] = *ptr;
        buf_index++;
      }
    } else {
      if (isspace(*ptr) == 0) {
        switch (*ptr) {
        case '{':
          token_buf[0] = '{';
          strcpy(token_buf, tokens[tokens_index]);
        }
      }
    }
  }
}

char **tokenize(const char *data) {
  char **tokens = create_tokens(MAX_TOKEN_NUMBER);
}
