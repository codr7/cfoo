#include "cfoo/id.h"
#include "cfoo/type.h"
#include "codr7/compare.h"

struct cf_type *cf_type_init(struct cf_type *type, struct cf_id *id) {
  type->id = id;
  
  type->compare_values = NULL;
  type->deinit_value = NULL;
  type->dump_value = NULL;
  type->is_value = NULL;

  return type;
}

void cf_type_deinit(struct cf_type *type) {}
