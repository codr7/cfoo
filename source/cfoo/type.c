#include <codr7/compare.h>

#include "cfoo/binding.h"
#include "cfoo/id.h"
#include "cfoo/thread.h"
#include "cfoo/type.h"
#include "cfoo/value.h"

struct cf_type *cf_type_init(struct cf_type *type,
			     struct cf_thread *thread,
			     const struct cf_id *id) {
  type->thread = thread;
  type->id = id;
  type->ref_count = 1;
  
  type->compare_values = NULL;
  type->deinit_value = NULL;
  type->dump_value = NULL;
  type->is_value = NULL;

  return type;
}

void cf_type_deinit(struct cf_type *type) {}

struct cf_type *cf_type_ref(struct cf_type *type) {
  type->ref_count++;
  return type;
}

void cf_type_deref(struct cf_type *type) {
  if (!--type->ref_count) {
    cf_type_deinit(type);
    c7_rbtree_remove(&type->thread->types, type->id);
  }
}

struct cf_type *cf_add_type(struct cf_thread *thread, const struct cf_id *id) {
  struct cf_type *t =
    cf_type_init(c7_rbtree_add(&thread->types, id), thread, id);

  cf_value_init(&cf_binding_init(c7_rbtree_add(&thread->bindings, id),
				 &thread->bindings,
				 id)->value,
		thread->meta_type ? thread->meta_type : t)->as_meta =
    cf_type_ref(t);
  
  return t;
}
