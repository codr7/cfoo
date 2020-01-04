#ifndef CFOO_TYPE_H
#define CFOO_TYPE_H

#include <stdbool.h>
#include <stdio.h>

struct cf_id;
struct cf_value;

struct cf_type {
  struct cf_id *id;

  enum c7_order (*compare_values)(const struct cf_value *x,
				  const struct cf_value *y);
  void (*deinit_value)(struct cf_value *v);
  void (*dump_value)(const struct cf_value *v, FILE *out);
  bool (*is_value)(const struct cf_value *x, const struct cf_value *y);
};

struct cf_type *cf_type_init(struct cf_type *type, struct cf_id *id);
void cf_type_deinit(struct cf_type *type);

#endif
