#ifndef CFOO_THREAD_H
#define CFOO_THREAD_H

#include <codr7/chan.h>
#include <codr7/deque.h>
#include <codr7/dqpool.h>
#include <codr7/rbpool.h>
#include <codr7/rbtree.h>
#include <stdbool.h>

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

  struct c7_rbpool method_pool;
  struct c7_rbtree methods;

  struct c7_rbpool binding_pool;
  struct c7_rbtree bindings;

  struct c7_dqpool op_pool;
  struct c7_chan chan;

  struct cf_type *int64_type, *meta_type, *method_type, *time_type;
};

struct cf_thread *cf_thread_new();
void cf_thread_free(struct cf_thread *thread);

#endif
