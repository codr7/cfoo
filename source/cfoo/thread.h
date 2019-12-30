#ifndef CFOO_THREAD_H
#define CFOO_THREAD_H

#include <codr7/deque.h>
#include <codr7/dqpool.h>
#include <codr7/rbpool.h>
#include <codr7/rbtree.h>
#include <stdbool.h>

struct cf_thread {
  bool debug;
  struct c7_dqpool error_pool, form_pool;
  struct c7_deque errors;
  struct c7_rbpool id_pool;
  struct c7_rbtree ids;
};

struct cf_thread *cf_thread_new();
void cf_thread_free(struct cf_thread *t);

#endif
