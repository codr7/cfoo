#ifndef CFOO_BINDING_H
#define CFOO_BINDING_H

#include "cfoo/value.h"

struct c7_rbtree;
struct cf_id;

struct cf_binding {
  struct c7_rbtree *owner;
  const struct cf_id *id;
  struct cf_value value;
};

struct cf_binding *cf_binding_init(struct cf_binding *binding,
				   struct c7_rbtree *owner,
				   const struct cf_id *id);

#endif
