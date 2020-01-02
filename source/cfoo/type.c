#include "cfoo/id.h"
#include "cfoo/type.h"
#include "codr7/compare.h"

struct cf_type *cf_type_init(struct cf_type *type, struct cf_id *id) {
  type->id = id;
  return type;
}

void cf_type_deinit(struct cf_type *type) {}

enum c7_order cf_type_compare(const void *key, const void *value) {
  return cf_id_compare(key, ((const struct cf_type *)value)->id);
}

