#include <codr7/rbtree.h>

#include "cfoo/binding.h"
#include "cfoo/method.h"
#include "cfoo/thread.h"
#include "cfoo/value.h"

struct cf_method *cf_method_init(struct cf_method *method,
				 struct cf_thread *thread,
				 const struct cf_id *id) {
  method->thread = thread;
  method->id = id;
  method->imp = NULL;
  method->ref_count = 1;
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
    c7_rbtree_remove(&method->thread->methods, method->id);
  }
}

struct cf_method *cf_add_method(struct cf_thread *thread,
				const struct cf_id *id) {
  struct cf_method *m = cf_method_init(c7_rbtree_add(&thread->methods, id),
				       thread, id);

  cf_value_init(&cf_binding_init(c7_rbtree_add(&thread->bindings, id),
				 &thread->bindings,
				 id)->value,
		thread->method_type)->as_method = cf_method_ref(m);
  
  return m;
}

bool cf_call(struct cf_method *method, struct cf_point point) {
  return method->imp(method->thread, point);
}
