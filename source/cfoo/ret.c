#include "cfoo/ret.h"

struct cf_ret cf_ret_index(uint8_t index) {
  return (struct cf_ret){.type = CF_RET_INDEX, .as_index = index};
}
