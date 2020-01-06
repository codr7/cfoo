#include "cfoo/arg.h"

struct cf_arg cf_arg_index(const struct cf_id *id, uint8_t index) {
  return (struct cf_arg){.type = CF_ARG_INDEX, .id = id, .as_index = index};
}

struct cf_arg cf_arg_type(const struct cf_id *id, struct cf_type *type) {
  return (struct cf_arg){.type = CF_ARG_TYPE, .id = id, .as_type = type};
}
