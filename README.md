# JSON Parser (C)

A lightweight JSON parser written in pure C.  
It tokenizes JSON input, builds an Abstract Syntax Tree (AST), and provides a path-based value fetcher.

## Features

- **Lexer / Tokenizer** — converts JSON text into a stream of tokens
- **Parser** — builds an AST from tokens (supports objects, arrays, and strings)
- **Pretty printer** — prints the AST in a readable nested format
- **Path fetcher** — retrieve values using a simple path syntax:
  - Nested objects: `nested.key1`
  - Array elements: `items.[0]`
  - Deep access: `users.[0].name`, `deep.level2.level3`

## Supported JSON Types

Currently supported:
- Objects (`{}`)
- Arrays (`[]`)
- Strings (`"..."`)

> Numbers, booleans (`true`/`false`), and `null` are not yet implemented.

## Project Structure

```
src
├── main.c          # Example usage + tests
├── lexer.c         # Parser + AST utilities
├── lexer.h         # AST type definitions
├── fetcher.h       # Path fetcher interface 
├── fetcher.c       # Fetch realization
├── tokenizer.h     # Token definitions
└── tokenizer.c     # Tokenizer realisation
```

## Building

```bash
xmake
```



## Usage Example

```c
#include "fetcher.h"

char *data = read_from_file("test/main.json");
Token *tokens = tokenize(data);
Object *ast = tokens_to_ast(tokens);

// Fetch values by path
char *name = (char *)fetch_from_ast("users.[0].name", ast);
char *age  = (char *)fetch_from_ast("users.[1].age", ast);
char *item = (char *)fetch_from_ast("items.[0]", ast);

printf("Name: %s\n", name);
printf("Age:  %s\n", age);
printf("Item: %s\n", item);

// Cleanup
destroy_object(ast);
free(tokens);
free(data);
```

## Path Syntax

| Path                  | Meaning                          |
|-----------------------|----------------------------------|
| `key`                 | Top-level object key             |
| `nested.key`          | Nested object access             |
| `items.[0]`           | First element of an array        |
| `users.[1].name`      | Field inside an object in array  |
| `deep.level2.level3`  | Deep nested access               |

## Memory Management

The library provides cleanup functions:
- `destroy_object(Object *obj)`
- `destroy_list(List *list)`
- `destroy_value(ValueTyped *vt)`

Always free the AST and the original token buffer after use.

## Limitations / TODO

- [ ] Support for numbers
- [ ] Support for `true` / `false` / `null`
- [ ] Better error reporting with line/column information
- [ ] Escape sequences in strings (`\n`, `\"`, etc.)
- [ ] Complete `ast_to_tokens` (serialization)
- [ ] More robust memory safety checks

## License

MIT (or whatever you prefer)

---

Made with ❤️ in pure C