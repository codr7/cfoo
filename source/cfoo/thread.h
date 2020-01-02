#ifndef CFOO_THREAD_H
#define CFOO_THREAD_H

#include <stdbool.h>

#include <codr7/chan.h>
#include <codr7/deque.h>
#include <codr7/dqpool.h>
#include <codr7/rbpool.h>
#include <codr7/rbtree.h>

struct cf_type;

struct cf_thread {
  bool debug;

  struct c7_dqpool error_pool;
  struct c7_deque errors;
  struct c7_dqpool form_pool;
  struct c7_rbpool id_pool;
  struct c7_rbtree ids;
  struct c7_rbpool type_pool;
  struct c7_rbtree types;
  struct c7_dqpool chan_pool;
  struct c7_chan chan;

  struct cf_type *int64_type;
};

struct cf_thread *cf_thread_new();
void cf_thread_free(struct cf_thread *t);

#endif
