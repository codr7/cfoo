#ifndef CFOO_THREAD_H
#define CFOO_THREAD_H

#include <codr7/chan.h>
#include <codr7/deque.h>
#include <codr7/deque_pool.h>
#include <codr7/tree_pool.h>
#include <codr7/tree.h>
#include <stdbool.h>

struct cf_point;
struct cf_type;

struct cf_thread {
  bool debug;

  struct c7_deque_pool error_pool;
  struct c7_deque errors;
  struct c7_deque_pool form_pool;

  struct c7_tree_pool id_pool;
  struct c7_tree ids;

  struct c7_tree_pool type_pool, type_parent_pool;
  struct c7_tree types;

  struct c7_tree_pool method_pool;
  struct c7_tree methods;

  struct c7_tree_pool binding_pool;
  struct c7_tree bindings;

  struct c7_deque_pool stack_pool;
  struct c7_deque stack;
  
  struct c7_deque_pool op_pool;
  struct c7_chan chan;

  uint64_t next_type_tag;

  struct cf_type *a_type,
    *bool_type,
    *int64_type,
    *meta_type, *method_type, *method_set_type,
    *time_type;
};

struct cf_thread *cf_thread_new();
void cf_thread_free(struct cf_thread *thread);

struct cf_value *cf_push(struct cf_thread *thread);
struct cf_value *cf_pop(struct cf_thread *thread);

void cf_dump_stack(struct cf_thread *thread,
		   const struct cf_point *point,
		   FILE *out);

#endif
