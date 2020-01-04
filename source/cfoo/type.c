#include "cfoo/id.h"
#include "cfoo/thread.h"
#include "cfoo/type.h"
#include "codr7/compare.h"

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

struct cf_type *cf_type_ref(struct cf_type *type) {
  type->ref_count++;
  return type;
}

void cf_type_deref(struct cf_type *type) {
  if (!--type->ref_count) {
    c7_rbtree_remove(&type->thread->types, type->id);
  }
}
