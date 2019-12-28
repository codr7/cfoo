#include <stdlib.h>
#include "cfoo/config.h"
#include "cfoo/error.h"
#include "cfoo/id.h"
#include "cfoo/thread.h"
#include "cfoo/form.h"

struct cf_thread *cf_thread_new() {
  struct cf_thread *t = malloc(sizeof(struct cf_thread));

  t->debug = false;
  cq_pool_init(&t->error_pool, CF_ERROR_BLOCK_SIZE, sizeof(struct cf_error));
  cq_pool_init(&t->form_pool, CF_FORM_BLOCK_SIZE, sizeof(struct cf_form));
  cq_deque_init(&t->errors, &t->error_pool);
  return t;
}

void cf_thread_free(struct cf_thread *t) {
  cq_deque_deinit(&t->errors);
  cq_pool_deinit(&t->form_pool);
  cq_pool_deinit(&t->error_pool);
  free(t);
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
