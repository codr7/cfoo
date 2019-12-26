#ifndef CFOO_THREAD_H
#define CFOO_THREAD_H

#include <stdbool.h>

#define CF_THREAD_BLOCK_SIZE 32

struct cq_deque;

struct cf_thread {
  bool debug;
};

struct cf_thread *cf_thread_new();

struct cq_deque *cf_threads();

#endif
