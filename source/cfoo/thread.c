#include <stdlib.h>
#include "cfoo/binding.h"
#include "cfoo/config.h"
#include "cfoo/error.h"
#include "cfoo/form.h"
#include "cfoo/id.h"
#include "cfoo/method.h"
#include "cfoo/thread.h"
#include "cfoo/type.h"

static enum c7_order compare_binding(const void *key, const void *value) {
  return cf_id_compare(key, ((const struct cf_binding *)value)->id);
}

static enum c7_order compare_id(const void *key, const void *value) {
  return c7_compare_str(key, ((const struct cf_id *)value)->name);
}

static enum c7_order compare_method(const void *key, const void *value) {
  return cf_id_compare(key, ((const struct cf_method *)value)->id);
}

static enum c7_order compare_type(const void *key, const void *value) {
  return cf_id_compare(key, ((const struct cf_type *)value)->id);
}

static struct cf_type *add_int64_type(struct cf_thread *thread) {
  struct cf_type *type = cf_add_type(thread, cf_id(thread, "Int64"));
  return type;
}

static void meta_deinit(struct cf_value *v) {
  cf_type_deref(v->as_meta);
}

static struct cf_type *add_meta_type(struct cf_thread *thread) {
  struct cf_type *type = cf_add_type(thread, cf_id(thread, "Meta"));
  type->deinit_value = meta_deinit;
  return type;
}

static void method_deinit(struct cf_value *v) {
  cf_method_deref(v->as_method);
}

static struct cf_type *add_method_type(struct cf_thread *thread) {
  struct cf_type *type = cf_add_type(thread, cf_id(thread, "Method"));
  type->deinit_value = method_deinit;
  return type;
}

static struct cf_type *add_time_type(struct cf_thread *thread) {
  struct cf_type *type = cf_add_type(thread, cf_id(thread, "Time"));
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

  c7_rbpool_init(&t->method_pool, CF_SLAB_SIZE, sizeof(struct cf_method));
  c7_rbtree_init(&t->methods, compare_method, &t->method_pool);

  c7_rbpool_init(&t->binding_pool, CF_SLAB_SIZE, sizeof(struct cf_binding));
  c7_rbtree_init(&t->bindings, compare_binding, &t->binding_pool);

  c7_chan_init(&t->chan, CF_SLAB_SIZE, sizeof(struct cf_value), 0);

  t->meta_type = NULL;
  t->meta_type = add_meta_type(t);
  t->method_type = add_method_type(t);
  t->int64_type = add_int64_type(t);
  t->time_type = add_time_type(t);
  return t;
}

void cf_thread_free(struct cf_thread *thread) {
  c7_chan_deinit(&thread->chan);
  
  c7_rbtree_do(&thread->bindings, b) {
    cf_binding_deinit(b);
  }
  
  c7_rbtree_clear(&thread->bindings);
  c7_rbpool_deinit(&thread->binding_pool);

  c7_rbtree_do(&thread->methods, m) {
    cf_method_deinit(m);
  }
  
  c7_rbtree_clear(&thread->methods);
  c7_rbpool_deinit(&thread->method_pool);

  c7_rbtree_do(&thread->types, t) {
    cf_type_deinit(t);
  }
  
  c7_rbtree_clear(&thread->types);
  c7_rbpool_deinit(&thread->type_pool);
  
  c7_rbtree_do(&thread->ids, id) {
    cf_id_deinit(id);
  }
  
  c7_rbtree_clear(&thread->ids);
  c7_rbpool_deinit(&thread->id_pool);

  c7_dqpool_deinit(&thread->form_pool);

  c7_deque_clear(&thread->errors);
  c7_dqpool_deinit(&thread->error_pool);

  free(thread);
}
