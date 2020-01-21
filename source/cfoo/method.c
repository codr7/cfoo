#include <codr7/tree.h>

#include "cfoo/binding.h"
#include "cfoo/method.h"
#include "cfoo/thread.h"
#include "cfoo/value.h"

struct cf_method *cf_method_init(struct cf_method *method,
				 struct cf_thread *thread,
				 const struct cf_id *id,
				 uint8_t arg_count, uint8_t ret_count,
				 va_list args) {
  method->thread = thread;
  method->id = id;
  method->arg_count = method->ret_count = 0;
  method->imp = NULL;
  method->ref_count = 1;

  for (uint8_t i = 0; i < arg_count; i++) {
    method->args[i] = va_arg(args, struct cf_arg);
  }

  for (uint8_t i = 0; i < arg_count; i++) {
    method->rets[i] = va_arg(args, struct cf_ret);
  }

  return method;
}

void cf_method_deinit(struct cf_method *method) {}

struct cf_method *cf_method_ref(struct cf_method *method) {
  method->ref_count++;
  return method;
}

void cf_method_deref(struct cf_method *method) {
  if (!--method->ref_count) {
    cf_method_deinit(method);
    c7_tree_remove(&method->thread->methods, method->id);
  }
}

struct cf_method *cf_add_method(struct cf_thread *thread,
				const struct cf_id *id,
				uint8_t arg_count, uint8_t ret_count,
				...) {
  va_list args;
  va_start(args, ret_count);

  struct cf_method *m = cf_method_init(c7_tree_add(&thread->methods, id),
				       thread, id, arg_count, ret_count, args);

  va_end(args);

  cf_value_init(&cf_binding_init(c7_tree_add(&thread->bindings, id),
				 &thread->bindings,
				 id)->value,
		thread->method_type)->as_method = cf_method_ref(m);
  
  return m;
}

bool cf_call(struct cf_method *method, const struct cf_point *point) {
  return method->imp(method->thread, point);
}
