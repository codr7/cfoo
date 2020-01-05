#include "cfoo/binding.h"

struct cf_binding *cf_binding_init(struct cf_binding *binding,
				   struct c7_rbtree *scope,
				   const struct cf_id *id) {
  binding->scope = scope;
  binding->id = id;
  return binding;
}

void cf_binding_deinit(struct cf_binding *binding) {
  cf_value_deinit(&binding->value);
}
