#include <stdlib.h>
#include "cfoo/config.h"
#include "cfoo/error.h"
#include "cfoo/form.h"
#include "cfoo/id.h"
#include "cfoo/thread.h"
#include "cfoo/type.h"
#include "cfoo/value.h"

static struct cf_type *add_type(struct cf_thread *t, const char *name) {
  struct cf_id *id = cf_id(t, name);
  return cf_type_init(c7_rbtree_add(&t->types, id), id);
}

struct cf_thread *cf_thread_new() {
  struct cf_thread *t = malloc(sizeof(struct cf_thread));
  t->debug = true;

  c7_dqpool_init(&t->error_pool, CF_SLAB_SIZE, sizeof(struct cf_error));
  c7_deque_init(&t->errors, &t->error_pool);

  c7_dqpool_init(&t->form_pool, CF_SLAB_SIZE, sizeof(struct cf_form));

  c7_rbpool_init(&t->id_pool, CF_SLAB_SIZE, sizeof(struct cf_id));
  c7_rbtree_init(&t->ids, cf_id_compare, &t->id_pool);

  c7_rbpool_init(&t->type_pool, CF_SLAB_SIZE, sizeof(struct cf_type));
  c7_rbtree_init(&t->types, cf_type_compare, &t->type_pool);

  c7_dqpool_init(&t->chan_pool, CF_SLAB_SIZE, sizeof(struct cf_value));
  c7_chan_init(&t->chan, &t->chan_pool, 0);

  t->int64_type = add_type(t, "Int64");
  return t;
}

static bool deinit_id(void *id, void *_) {
  cf_id_deinit(id);
  return true;
}

static bool deinit_type(void *type, void *_) {
  cf_type_deinit(type);
  return true;
}

void cf_thread_free(struct cf_thread *t) {
  c7_chan_deinit(&t->chan);
  c7_dqpool_deinit(&t->chan_pool);
  
  c7_rbtree_while(&t->types, deinit_type, NULL);
  c7_rbpool_deinit(&t->type_pool);
  
  c7_rbtree_while(&t->ids, deinit_id, NULL);
  c7_rbpool_deinit(&t->id_pool);

  c7_dqpool_deinit(&t->form_pool);
  c7_dqpool_deinit(&t->error_pool);

  free(t);
}
