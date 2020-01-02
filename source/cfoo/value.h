#ifndef CFOO_VALUE_H
#define CFOO_VALUE_H

#include <stdint.h>

struct cf_value {
  struct cf_type *type;
  
  union {
    int64_t as_int64;
  };
};

struct cf_value *cf_value_init(struct cf_value *value, struct cf_type *type);
void cf_value_deinit(struct cf_value *value);

#endif
