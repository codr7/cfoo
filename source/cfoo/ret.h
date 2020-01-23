#ifndef CFOO_RET_H
#define CFOO_RET_H

#include <stdint.h>

enum cf_ret_type {CF_RINDEX, CF_RTYPE};
  
struct cf_ret {
  enum cf_ret_type type;
  
  union {
    uint8_t as_index;
    struct cf_type *as_type;
  };
};

struct cf_ret cf_ret_index(uint8_t index);
struct cf_ret cf_ret_type(struct cf_type *type);
void cf_ret_deinit(struct cf_ret *ret);

#endif
