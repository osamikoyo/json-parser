#include "tokenizer.h"

typedef struct Object Object;
typedef struct Pair Pair;

enum ValueType {
  OBJECT,
  VALUE,
};

typedef union {
  char *value;
  Object *obj;
} Value;

struct Object {
  Pair *pairs;
};

struct Pair {
  char *key;
  enum ValueType type;
  Value value;
};

Object *tokens_to_ast(Token *tokens);
