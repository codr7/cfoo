#include <libceque/deque.h>
#include "cfoo/config.h"
#include "cfoo/thread.h"
#include "cfoo/token.h"

struct cf_thread *cf_thread_new() {
  cf_threads_lock();
  struct cf_thread *t = cq_deque_push_back(cf_threads());
  cf_threads_unlock();
  t->debug = false;
  cq_pool_init(&t->token_pool, CF_TOKEN_BLOCK_SIZE, sizeof(struct cf_token));
  return t;
}

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
