#include "cfoo/value.h"

struct cf_value *cf_value_init(struct cf_value *value, struct cf_type *type) {
  value->type = type;
  return value;
}

void cf_value_deinit(struct cf_value *value) {}

