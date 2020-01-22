#ifndef CFOO_TYPE_H
#define CFOO_TYPE_H

#include <codr7/tree.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define cf_add_type(thread, id, ...)		\
  _cf_add_type(thread, id, ##__VA_ARGS__, NULL)

struct cf_id;
struct cf_point;
struct cf_value;

struct cf_type {
  struct cf_thread *thread;
  const struct cf_id *id;
  struct c7_tree parents;
  uint16_t ref_count;

  enum c7_order (*compare_value)(const struct cf_value *x,
				 const struct cf_value *y);

  void (*copy_value)(struct cf_value *dst, struct cf_value *src);
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

void cf_derive(struct cf_type *child, struct cf_type *parent);
struct cf_type *cf_root(struct cf_type *child, struct cf_type *parent);
			
struct cf_type *_cf_add_type(struct cf_thread *thread, const struct cf_id *id, ...);

#endif
