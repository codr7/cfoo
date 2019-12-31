#include <stdlib.h>
#include "cfoo/config.h"
#include "cfoo/error.h"
#include "cfoo/id.h"
#include "cfoo/thread.h"
#include "cfoo/form.h"

struct cf_thread *cf_thread_new() {
  struct cf_thread *t = malloc(sizeof(struct cf_thread));
  t->debug = false;
  c7_dqpool_init(&t->error_pool, CF_ERROR_SLAB_SIZE, sizeof(struct cf_error));
  c7_deque_init(&t->errors, &t->error_pool);
  c7_dqpool_init(&t->form_pool, CF_FORM_SLAB_SIZE, sizeof(struct cf_form));
  c7_rbpool_init(&t->id_pool, CF_ID_SLAB_SIZE, sizeof(struct cf_id));
  c7_rbtree_init(&t->ids, cf_id_compare, &t->id_pool);
  return t;
}

static bool deinit_id(void *id, void *_) {
  cf_id_deinit(id);
  return true;
}

void cf_thread_free(struct cf_thread *t) {
  c7_rbtree_while(&t->ids, deinit_id, NULL);
  c7_rbpool_deinit(&t->id_pool);
  c7_dqpool_deinit(&t->form_pool);
  c7_dqpool_deinit(&t->error_pool);
  free(t);
}
