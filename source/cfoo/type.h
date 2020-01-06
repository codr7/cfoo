#ifndef CFOO_TYPE_H
#define CFOO_TYPE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

struct cf_id;
struct cf_point;
struct cf_value;

struct cf_type {
  struct cf_thread *thread;
  const struct cf_id *id;
  uint16_t ref_count;
  
  enum c7_order (*compare_value)(const struct cf_value *x,
				 const struct cf_value *y);
  void (*deinit_value)(struct cf_value *v);
  
  bool (*dump_value)(struct cf_thread *thread,
		     const struct cf_point *point,
		     const struct cf_value *v,
		     FILE *out);

  bool (*is_value)(const struct cf_value *x, const struct cf_value *y);
};

struct cf_type *cf_type_init(struct cf_type *type,
			     struct cf_thread *thread,
			     const struct cf_id *id);

void cf_type_deinit(struct cf_type *type);

struct cf_type *cf_type_ref(struct cf_type *type);
void cf_type_deref(struct cf_type *type);

struct cf_type *cf_add_type(struct cf_thread *thread, const struct cf_id *id);

#endif
