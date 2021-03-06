#ifndef CFOO_METHOD_H
#define CFOO_METHOD_H

#include <codr7/list.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "cfoo/arg.h"
#include "cfoo/config.h"
#include "cfoo/point.h"
#include "cfoo/ret.h"

struct c7_tree;
struct cf_thread;

typedef bool (*cf_method_imp)(struct cf_thread *thread, const struct cf_point *);

struct cf_method {
  struct cf_thread *thread;
  struct cf_point point;
  struct cf_method_set *set;
  struct c7_list set_item;
  const struct cf_id *id;
  
  struct cf_arg args[CF_MAX_ARGS];
  struct cf_ret rets[CF_MAX_RETS];
  uint8_t ret_count;
  
  cf_method_imp imp;
  uint16_t ref_count;
};

struct cf_method *cf_method_init(struct cf_method *method,
				 struct cf_thread *thread,
				 const struct cf_point *point,
				 const struct cf_id *id,
				 struct cf_method_set *set,
				 struct cf_arg *args,
				 uint8_t ret_count, struct cf_ret *rets);

void cf_method_deinit(struct cf_method *method);

struct cf_method *cf_method_ref(struct cf_method *method);
void cf_method_deref(struct cf_method *method);

struct cf_method *cf_add_method(struct cf_thread *thread,
				const struct cf_point *point,
				struct cf_method_set *set,
				struct cf_arg *args,
				uint8_t ret_count, struct cf_ret *rets);

struct cf_method *cf_bind_method(struct cf_thread *thread,
				 const struct cf_point *point,
				 const struct cf_id *id,
				 uint8_t arg_count, struct cf_arg *args,
				 uint8_t ret_count, struct cf_ret *rets);

bool cf_applicable(struct cf_method *method, struct cf_value *stack_pointer);
bool cf_call(struct cf_method *method, const struct cf_point *point);

#endif
