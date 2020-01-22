#include <codr7/compare.h>
#include <stdarg.h>

#include "cfoo/binding.h"
#include "cfoo/id.h"
#include "cfoo/thread.h"
#include "cfoo/type.h"
#include "cfoo/type_parent.h"
#include "cfoo/value.h"

static enum c7_order compare_parent(const void *key, const void *value) {
  return c7_compare(key, ((const struct cf_type_parent *)value)->type);
}

struct cf_type *cf_type_init(struct cf_type *type,
			     struct cf_thread *thread,
			     const struct cf_id *id) {
  type->thread = thread;
  type->id = id;
  type->tag = thread->next_type_tag++;
  c7_tree_init(&type->parents, compare_parent, &thread->type_parent_pool);
  type->ref_count = 1;
  
  type->compare_value = NULL;
  type->copy_value = NULL;
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
    c7_tree_do(&type->parents, p) {
      cf_type_parent_deinit(p);
    }

    c7_tree_clear(&type->parents);
    cf_type_deinit(type);
    c7_tree_remove(&type->thread->types, type->id);
  }
}

static void derive_type(struct cf_type *child, struct cf_type *root, struct cf_type *type) {
  struct cf_type_parent *p = c7_tree_add(&child->parents, type);

  if (p) {
    cf_type_parent_init(p, root, type);
    
    c7_tree_do(&type->parents, pp) {
      derive_type(child, root, ((struct cf_type_parent *)pp)->type);
    }
  }
}

void cf_derive(struct cf_type *child, struct cf_type *root) {
  derive_type(child, root, root);
}

struct cf_type *cf_find_root(struct cf_type *x, struct cf_type *y, bool strict) {
  if (x == y) {
    return x;
  }

  bool x_parent = !strict &&  x->tag < y->tag;
  struct cf_type *pt = x_parent ? x : y, *ct = x_parent ? y : x;
  struct cf_type_parent *p = c7_tree_find(&ct->parents, pt);
  return p ? p->root : NULL;
}

struct cf_type *_cf_add_type(struct cf_thread *thread, const struct cf_id *id, ...) {
  struct cf_type *t =
    cf_type_init(c7_tree_add(&thread->types, id), thread, id);

  cf_value_init(&cf_binding_init(c7_tree_add(&thread->bindings, id),
				 &thread->bindings,
				 id)->value,
		thread->meta_type ? thread->meta_type : t)->as_meta =
    cf_type_ref(t);
  
  va_list parents;
  va_start(parents, id);

  for (;;) {
    struct cf_type *pt = va_arg(parents, struct cf_type *);

    if (!pt) {
      break;
    }

    cf_derive(t, pt);
  }
  
  va_end(parents);
  return t;
}
