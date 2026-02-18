#define TOKEN_MAX_SIZE 32
#define MAX_TOKEN_NUMBER 250

enum State {
  NORMAL,
  IN_STRING,
  IN_LIST,
};

char **tokenize(const char *data);
