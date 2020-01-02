#ifndef CFOO_FORM_H
#define CFOO_FORM_H

#include <codr7/deque.h>

struct cf_id;
struct cf_thread;

enum cf_form_type {CF_ID, CF_PARAMS};

struct cf_form {
  enum cf_form_type type;
  
  union {
    struct cf_id *as_id;
    struct c7_deque as_params;
  };
};

struct cf_form *cf_form_init(struct cf_form *form,
			     enum cf_form_type type,
			     struct cf_thread *t);

void cf_form_deinit(struct cf_form *form);

void cf_clear_forms(struct c7_deque *forms);

#endif
