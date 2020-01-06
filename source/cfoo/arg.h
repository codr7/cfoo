#ifndef CFOO_ARG_H
#define CFOO_ARG_H

#include "cfoo/value.h"

enum cf_arg_type {CF_ARG_INDEX, CF_ARG_TYPE, CF_ARG_VALUE};
  
struct cf_arg {
  enum cf_arg_type type;
  const struct cf_id *id;

  union {
    uint8_t as_index;
    struct cf_type *as_type;
    struct cf_value as_value;
  };
};

struct cf_arg cf_arg_index(const struct cf_id *id, uint8_t index);
struct cf_arg cf_arg_type(const struct cf_id *id, struct cf_type *type);

#endif
