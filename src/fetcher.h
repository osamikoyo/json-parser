#ifndef FETCHER_H
#define FETCHER_H

#include "lexer.h"

void *fetch_from_ast(const char *key, Object *ast);
Object *fetch_object_from_ast(const char *key, Object *ast);

#endif // FETCHER_H
