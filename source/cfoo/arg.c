#include "cfoo/arg.h"
#include "cfoo/type.h"

struct cf_arg cf_arg_index(const struct cf_id *id, uint8_t index) {
  return (struct cf_arg){.type = CF_ARG_INDEX, .id = id, .as_index = index};
}

struct cf_arg cf_arg_type(const struct cf_id *id, struct cf_type *type) {
  return (struct cf_arg){.type = CF_ARG_TYPE, .id = id, .as_type = cf_type_ref(type)};
}

void cf_arg_deinit(struct cf_arg *arg) {
  switch(arg->type) {
  case CF_ARG_TYPE:
    cf_type_deref(arg->as_type);
    break;
  case CF_ARG_VALUE:
    cf_value_deinit(&arg->as_value);
    break;
  default:
    break;
  }
}

