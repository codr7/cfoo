#include <stdint.h>

#include "cfoo/method.h"
#include "cfoo/method_set.h"
#include "cfoo/type.h"

struct cf_method_set *cf_method_set_init(struct cf_method_set *set, const struct cf_id *id) {
  set->id = id;
  c7_list_init(&set->methods);
  return set;
}

void cf_method_set_deinit(struct cf_method_set *set) {}

void cf_method_set_add(struct cf_method_set *set, struct cf_method *x) {
  c7_list_do(&set->methods, _y) {
    struct cf_method *y = c7_baseof(_y, struct cf_method, set);

    for (uint8_t i = 0; i < c7_min(x->arg_count, y->arg_count); i++) {
      struct cf_arg *xa = x->args + i, *ya = y->args + i;
      struct cf_type *xt = NULL, *yt = NULL;
      
      switch (xa->type) {
      case CF_ARG_INDEX:
	xt = x->args[xa->as_index].as_type;
	break;
      case CF_ARG_TYPE:
	xt = xa->as_type;
	break;
      case CF_ARG_VALUE:
	xt = xa->as_value.type;
	break;
      }

      switch (ya->type) {
      case CF_ARG_INDEX:
	yt = y->args[ya->as_index].as_type;
	break;
      case CF_ARG_TYPE:
	yt = ya->as_type;
	break;
      case CF_ARG_VALUE:
	yt = ya->as_value.type;
	break;
      }
      
      if ((ya->type == CF_ARG_VALUE && (xa->type != CF_ARG_VALUE || xt->tag < yt->tag))) {
	c7_list_insert(&y->set, &x->set);
	return;
      }
    }
  }

  c7_list_push_back(&set->methods, &x->set);
}
