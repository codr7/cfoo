#include "cfoo/type.h"
#include "cfoo/type_parent.h"

void cf_type_parent_init(struct cf_type_parent *parent, struct cf_type *root, struct cf_type *type) {
  parent->root = cf_type_ref(root);
  parent->type = cf_type_ref(type);
}

void cf_type_parent_deinit(struct cf_type_parent *parent) {
  cf_type_deref(parent->root);
  cf_type_deref(parent->type);
}
