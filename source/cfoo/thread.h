#ifndef CFOO_THREAD_H
#define CFOO_THREAD_H

#include <libceque/deque.h>
#include <libceque/pool.h>
#include <stdbool.h>

struct cf_thread {
  bool debug;
  struct cq_pool error_pool, form_pool;
  struct cq_deque errors;
};

struct cf_thread *cf_thread_new();
void cf_thread_free(struct cf_thread *t);

#endif
