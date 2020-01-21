#include "cfoo/ret.h"
#include "cfoo/type.h"

struct cf_ret cf_ret_index(uint8_t index) {
  return (struct cf_ret){.type = CF_RET_INDEX, .as_index = index};
}

struct cf_ret cf_ret_type(struct cf_type *type) {
  return (struct cf_ret){.type = CF_RET_TYPE, .as_type = cf_type_ref(type)};
}

void cf_ret_deinit(struct cf_ret *ret) {
  if (ret->type == CF_RET_TYPE) {
    cf_type_deref(ret->as_type);
  }
}

