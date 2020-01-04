#include "cfoo/binding.h"

struct cf_binding *cf_binding_init(struct cf_binding *binding,
				   struct c7_rbtree *owner,
				   const struct cf_id *id) {
  binding->owner = owner;
  binding->id = id;
  return binding;
}
