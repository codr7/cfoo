#ifndef CFOO_METHOD_H
#define CFOO_METHOD_H

#include <stdint.h>

struct cf_method {
  struct cf_thread *thread;
  const struct cf_id *id;
  uint16_t ref_count;
};

struct cf_method *cf_method_init(struct cf_method *method,
				 struct cf_thread *thread,
				 const struct cf_id *id);

void cf_method_deinit(struct cf_method *method);

struct cf_method *cf_method_ref(struct cf_method *method);
void cf_method_deref(struct cf_method *method);

struct cf_method *cf_add_method(struct cf_thread *thread,
				const struct cf_id *id);

#endif
