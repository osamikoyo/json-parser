#include "tokenizer.h"

typedef struct Object Object;
typedef struct Pair Pair;
typedef union Value Value;
typedef struct List List;

enum ValueType {
  OBJECT,
  STRING_VALUE,
  LIST,
};

 union Value{
  char *str;
  Object *obj;
  List* list;
};

struct List{
  int len;
  Value *elems;
};

struct Object {
  Pair *pairs;
};

struct Pair {
  char *key;
  enum ValueType type;
  Value value;
};

Object *tokens_to_ast(Token *tokens);
