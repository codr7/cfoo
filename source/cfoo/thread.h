#ifndef CFOO_THREAD_H
#define CFOO_THREAD_H

#include <libceque/deque.h>
#include <libceque/pool.h>
#include <libceque/set.h>
#include <stdbool.h>

struct cf_thread {
  bool debug;
  struct cq_pool error_pool, form_pool, id_pool;
  struct cq_deque errors;
  struct cq_set ids;
};

struct cf_thread *cf_thread_new();

struct cq_deque *cf_threads();
void cf_threads_lock();
void cf_threads_unlock();

#endif
