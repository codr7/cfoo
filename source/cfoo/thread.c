#include <stdlib.h>
#include "cfoo/binding.h"
#include "cfoo/config.h"
#include "cfoo/error.h"
#include "cfoo/form.h"
#include "cfoo/id.h"
#include "cfoo/thread.h"
#include "cfoo/type.h"
#include "cfoo/value.h"

static enum c7_order compare_id(const void *key, const void *value) {
  return c7_compare_str(key, ((const struct cf_id *)value)->name);
}

static enum c7_order compare_type(const void *key, const void *value) {
  return cf_id_compare(key, ((const struct cf_type *)value)->id);
}

static enum c7_order compare_binding(const void *key, const void *value) {
  return cf_id_compare(key, ((const struct cf_binding *)value)->id);
}

static struct cf_type *add_type(struct cf_thread *t, const char *name) {
  struct cf_id *id = cf_id(t, name);
  struct cf_type *type = cf_type_init(c7_rbtree_add(&t->types, id), t, id);

  cf_value_init(&cf_binding_init(c7_rbtree_add(&t->bindings, id),
				 &t->bindings,
				 id)->value,
		t->meta_type ? t->meta_type : type)->as_meta = cf_type_ref(type);
  
  return type;
}

static struct cf_type *add_int64_type(struct cf_thread *t) {
  struct cf_type *type = add_type(t, "Int64");
  return type;
}

static void deinit_meta(struct cf_value *v) {
  cf_type_deref(v->as_meta);
}

static struct cf_type *add_meta_type(struct cf_thread *t) {
  struct cf_type *type = add_type(t, "Meta");
  type->deinit_value = deinit_meta;
  return type;
}

static struct cf_type *add_time_type(struct cf_thread *t) {
  struct cf_type *type = add_type(t, "Time");
  return type;
}

struct cf_thread *cf_thread_new() {
  struct cf_thread *t = malloc(sizeof(struct cf_thread));
  t->debug = true;

  c7_dqpool_init(&t->error_pool, CF_SLAB_SIZE, sizeof(struct cf_error));
  c7_deque_init(&t->errors, &t->error_pool);

  c7_dqpool_init(&t->form_pool, CF_SLAB_SIZE, sizeof(struct cf_form));

  c7_rbpool_init(&t->id_pool, CF_SLAB_SIZE, sizeof(struct cf_id));
  c7_rbtree_init(&t->ids, compare_id, &t->id_pool);

  c7_rbpool_init(&t->type_pool, CF_SLAB_SIZE, sizeof(struct cf_type));
  c7_rbtree_init(&t->types, compare_type, &t->type_pool);

  c7_rbpool_init(&t->binding_pool, CF_SLAB_SIZE, sizeof(struct cf_binding));
  c7_rbtree_init(&t->bindings, compare_binding, &t->binding_pool);

  c7_chan_init(&t->chan, sizeof(struct cf_value), 0);

  t->meta_type = NULL;
  t->meta_type = add_meta_type(t);
  t->int64_type = add_int64_type(t);
  t->time_type = add_time_type(t);
  return t;
}

static bool deinit_binding(void *type, void *_) {
  cf_value_deinit(&((struct cf_binding *)type)->value);
  return true;
}

static bool deinit_id(void *id, void *_) {
  cf_id_deinit(id);
  return true;
}

static bool deinit_type(void *type, void *_) {
  cf_type_deref(type);
  return true;
}

void cf_thread_free(struct cf_thread *t) {
  c7_chan_deinit(&t->chan);
  
  c7_rbtree_while(&t->bindings, deinit_binding, NULL);
  c7_rbtree_clear(&t->bindings);
  c7_rbpool_deinit(&t->binding_pool);

  c7_rbtree_while(&t->types, deinit_type, NULL);
  c7_rbtree_clear(&t->types);
  c7_rbpool_deinit(&t->type_pool);
  
  c7_rbtree_while(&t->ids, deinit_id, NULL);
  c7_rbtree_clear(&t->ids);
  c7_rbpool_deinit(&t->id_pool);

  c7_dqpool_deinit(&t->form_pool);

  c7_deque_clear(&t->errors);
  c7_dqpool_deinit(&t->error_pool);

  free(t);
}
