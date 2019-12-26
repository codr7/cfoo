#include <libceque/deque.h>
#include <libceque/pool.h>
#include "cfoo/thread.h"

struct cq_deque *cf_threads() {
  static __thread struct cq_pool p;
  static __thread struct cq_deque d;
  static __thread bool init = true;

  if (init) {
    cq_pool_init(&p, CF_THREAD_BLOCK_SIZE, sizeof(struct cf_thread));
    cq_deque_init(&d, &p);
    init = false;
  }

  return &d;
}

struct cf_thread *cf_thread_new() {
  struct cf_thread *thread = cq_deque_push_back(cf_threads());
  thread->debug = false;
  return thread;
}
