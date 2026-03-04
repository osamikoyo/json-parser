#include <stddef.h>
#define TOKEN_MAX_SIZE 32
#define MAX_TOKEN_NUMBER 250

enum TokenType {
  LBRACE,
  RBRACE,
  L_ARR_BRACE,
  R_ARR_BRACE,
  STRING,
  COLON,
  NUMBER,
  COMMA,
  EOF_TOKEN,
  UNKNOWN,
};

typedef struct Token {
  enum TokenType type;
  const char *start;
  size_t len;
} Token;

Token *tokenize(const char *data);