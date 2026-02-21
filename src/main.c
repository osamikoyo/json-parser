#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>

char *read_from_file(const char *filename) {
  FILE *f = fopen(filename, "rb");
  if (!f) {
    perror("fopen");
    return NULL;
  }

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);

  if (size < 0) {
    fclose(f);
    return NULL;
  }

  char *buf = malloc(size + 1);
  if (!buf) {
    fclose(f);
    return NULL;
  }

  size_t read = fread(buf, 1, size, f);
  fclose(f);

  if (read != (size_t)size) {
    free(buf);
    return NULL;
  }

  buf[size] = '\0';
  return buf;
}

int display_token(const Token *tkn) {
  if (tkn == NULL || tkn->start == NULL || tkn->len == 0) {
    printf("(null/empty) ");
    return 0;
  }
  printf("%.*s ", (int)tkn->len, tkn->start);

  return 1;
}

int main() {
  const char *file_name = "test/main.json";

  char *data = read_from_file(file_name);
  if (!data) {
    fprintf(stderr, "Failed to read file\n");
    return 1;
  }

  printf("token number: %d\n", get_token_numbers(data));

  Token *tokens = tokenize(data);
  if (!tokens) {
    fprintf(stderr, "Tokenization failed\n");
    free(data);
    return 1;
  }

  Token *current = tokens;
  while (current->type != EOF_TOKEN || current->len != 0) {
    if (display_token(current) == 0) {
      break;
    }
    current++;
  }

  free(tokens);
  free(data);

  return 0;
}
