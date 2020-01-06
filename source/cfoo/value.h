#ifndef CFOO_VALUE_H
#define CFOO_VALUE_H

#include <codr7/compare.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

struct cf_point;
struct cf_thread;

struct cf_value {
  struct cf_type *type;
  
  union {
    int64_t as_int64;
    struct cf_type *as_meta;
    struct cf_method *as_method;
    struct timespec as_time;
  };
};

struct cf_value *cf_value_init(struct cf_value *value, struct cf_type *type);
void cf_value_deinit(struct cf_value *value);

enum c7_order cf_value_compare(const struct cf_value *x,
			       const struct cf_value *y);

bool cf_value_dump(struct cf_thread *thread,
		   const struct cf_point *point,
		   const struct cf_value *v,
		   FILE *out);

bool cf_value_is(const struct cf_value *x, const struct cf_value *y);

#endif
