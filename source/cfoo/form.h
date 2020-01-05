#ifndef CFOO_FORM_H
#define CFOO_FORM_H

#include <codr7/deque.h>

#include <cfoo/point.h>
#include <cfoo/value.h>

struct cf_id;
struct cf_thread;

enum cf_form_type {CF_ID, CF_GROUP, CF_VALUE};

struct cf_form {
  enum cf_form_type type;
  struct cf_point point;
  
  union {
    const struct cf_id *as_id;
    struct c7_deque as_group;
    struct cf_value as_value;
  };
};

struct cf_form *cf_form_init(struct cf_form *form,
			     enum cf_form_type type,
			     struct cf_point point,
			     struct cf_thread *thread);

void cf_form_deinit(struct cf_form *form);

void cf_clear_forms(struct c7_deque *forms);

#endif
