#include "cfoo/method_set.h"

struct cf_method_set *cf_method_set_init(struct cf_method_set *set, const struct cf_id *id) {
  set->id = id;
  return set;
}

void cf_method_set_deinit(struct cf_method_set *set) {}
