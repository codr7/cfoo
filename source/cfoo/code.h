#ifndef CFOO_CODE_H
#define CFOO_CODE_H

#include <stdbool.h>

#include "codr7/deque.h"

struct c7_tree;
struct cf_thread;

struct cf_code {
  struct cf_thread *thread;
  struct c7_deque ops;
};

struct cf_code *cf_code_init(struct cf_code *code, struct cf_thread *thread);
void cf_code_deinit(struct cf_code *code);
void cf_code_clear(struct cf_code *code);

bool cf_compile(struct c7_deque *in, struct c7_tree *bindings, struct cf_code *out);
bool cf_eval(struct cf_code *in);

#endif
