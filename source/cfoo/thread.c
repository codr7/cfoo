#include <string.h>
#include "cfoo/config.h"
#include "cfoo/error.h"
#include "cfoo/id.h"
#include "cfoo/thread.h"
#include "cfoo/form.h"

static enum cq_order compare_id(const void *key, const void *value) {
  const char *x = key, *y = ((const struct cf_id *)value)->name;

  switch(strcmp(x, y)) {
  case -1:
    return CQ_LT;
  case 1:
    return CQ_GT;
  default:
    break;
  }

  return 0;
}

struct cf_thread *cf_thread_new() {
  cf_threads_lock();
  struct cf_thread *t = cq_deque_push_back(cf_threads());
  cf_threads_unlock();
  t->debug = false;
  cq_pool_init(&t->error_pool, CF_ERROR_BLOCK_SIZE, sizeof(struct cf_error));
  cq_pool_init(&t->form_pool, CF_FORM_BLOCK_SIZE, sizeof(struct cf_form));
  cq_pool_init(&t->id_pool, CF_ID_BLOCK_SIZE, sizeof(struct cf_id));
  cq_deque_init(&t->errors, &t->error_pool);
  cq_set_init(&t->ids, compare_id, &t->id_pool);
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
