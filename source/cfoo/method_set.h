#ifndef CFOO_METHOD_SET_H
#define CFOO_METHOD_SET_H

#include <codr7/list.h>

struct cf_method;

struct cf_method_set {
  const struct cf_id *id;
  uint8_t arg_count;
  uint16_t count;

  struct c7_list items;
};

struct cf_method_set *cf_method_set_init(struct cf_method_set *set,
					 const struct cf_id *id,
					 uint8_t arg_count);

void cf_method_set_deinit(struct cf_method_set *set);

void cf_method_set_add(struct cf_method_set *set, struct cf_method *x);

struct cf_method *cf_dispatch(struct cf_method_set *set, struct cf_value *stack_pointer);

#endif
