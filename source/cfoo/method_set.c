#include <stdint.h>

#include "cfoo/method.h"
#include "cfoo/method_set.h"
#include "cfoo/type.h"

struct cf_method_set *cf_method_set_init(struct cf_method_set *set,
					 const struct cf_id *id,
					 uint8_t arg_count) {
  set->id = id;
  set->arg_count = arg_count;
  set->count = 0;

  c7_list_init(&set->items);
  return set;
}

void cf_method_set_deinit(struct cf_method_set *set) {}

void cf_method_set_add(struct cf_method_set *set, struct cf_method *x) {
  c7_list_do(&set->items, _y) {
    struct cf_method *y = c7_baseof(_y, struct cf_method, set_item);

    for (uint8_t i = 0; i < set->arg_count; i++) {
      struct cf_arg *xa = x->args + i, *ya = y->args + i;
      struct cf_type *xt = NULL, *yt = NULL;
      
      switch (xa->type) {
      case CF_AINDEX:
	xt = x->args[xa->as_index].as_type;
	break;
      case CF_ATYPE:
	xt = xa->as_type;
	break;
      case CF_AVALUE:
	xt = xa->as_value.type;
	break;
      }

      switch (ya->type) {
      case CF_AINDEX:
	yt = y->args[ya->as_index].as_type;
	break;
      case CF_ATYPE:
	yt = ya->as_type;
	break;
      case CF_AVALUE:
	yt = ya->as_value.type;
	break;
      }
      
      if ((ya->type == CF_AVALUE && (xa->type != CF_AVALUE || xt->tag < yt->tag))) {
	c7_list_insert(&y->set_item, &x->set_item);
	set->count++;
	return;
      }
    }
  }

  c7_list_push_back(&set->items, &x->set_item);
  set->count++;
}

struct cf_method *cf_dispatch(struct cf_method_set *set, struct cf_value *stack_pointer) {
  c7_list_do(&set->items, _m) {
    struct cf_method *m = c7_baseof(_m, struct cf_method, set_item);
    
    if (cf_applicable(m, stack_pointer)) {
      return m;
    }
  }

  return NULL;
}
