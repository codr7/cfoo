#ifndef CFOO_THREAD_H
#define CFOO_THREAD_H

#include <libceque/deque.h>
#include <libceque/pool.h>
#include <stdbool.h>

struct cf_thread {
  bool debug;
  struct cq_pool error_pool, token_pool;
  struct cq_deque errors;
};

struct cf_thread *cf_thread_new();

struct cq_deque *cf_threads();
void cf_threads_lock();
void cf_threads_unlock();

#endif
