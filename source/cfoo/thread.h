#ifndef CFOO_THREAD_H
#define CFOO_THREAD_H

#include <stdbool.h>

struct cq_deque;

struct cf_thread {
  bool debug;
};

struct cf_thread *cf_thread_new();

struct cq_deque *cf_threads();
void cf_threads_lock();
void cf_threads_unlock();

#endif
