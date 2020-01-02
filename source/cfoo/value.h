#ifndef CFOO_VALUE_H
#define CFOO_VALUE_H

#include <stdint.h>

struct cf_value {
  union {
    int64_t as_int64;
  };
};

#endif
