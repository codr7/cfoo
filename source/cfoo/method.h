#ifndef CFOO_METHOD_H
#define CFOO_METHOD_H

#include <stdbool.h>
#include <stdint.h>

#include "cfoo/point.h"

struct cf_thread;

typedef bool (*cf_method_imp)(struct cf_thread *thread, struct cf_point);

struct cf_method {
  struct cf_thread *thread;
  const struct cf_id *id;
  cf_method_imp imp;
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

bool cf_call(struct cf_method *method, struct cf_point point);

#endif
