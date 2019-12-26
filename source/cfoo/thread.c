#include <libceque/deque.h>
#include <libceque/pool.h>
#include "cfoo/config.h"
#include "cfoo/thread.h"

struct cq_deque *cf_threads() {
  static struct cq_pool p;
  static struct cq_deque d;
  static bool init = true;

  if (init) {
    cq_pool_init(&p, CF_THREAD_BLOCK_SIZE, sizeof(struct cf_thread));
    cq_deque_init(&d, &p);
    init = false;
  }

  return &d;
}

void cf_threads_lock() {
}

void cf_threads_unlock() {
}

struct cf_thread *cf_thread_new() {
  cf_threads_lock();
  struct cf_thread *thread = cq_deque_push_back(cf_threads());
  cf_threads_unlock();
  thread->debug = false;
  return thread;
}
