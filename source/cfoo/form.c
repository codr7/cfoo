#include <stdio.h>
#include "cfoo/form.h"
#include "cfoo/thread.h"

struct cf_form *cf_form_init(struct cf_form *form,
			     enum cf_form_type type,
			     struct cf_point point,
			     struct cf_thread *thread) {
  form->type = type;
  form->point = point;
  
  switch (type) {
  case CF_GROUP:
    c7_deque_init(&form->as_group, &thread->form_pool);
    break;
  default:
    break;
  }
  
  return form;
}

void cf_form_deinit(struct cf_form *form) {
  switch (form->type) {
  case CF_GROUP:
    cf_clear_forms(&form->as_group);
    break;
  case CF_VALUE:
    cf_value_deinit(&form->as_value);
  default:
    break;
  }
}

void cf_clear_forms(struct c7_deque *forms) {
    c7_deque_do(forms, f) {
      cf_form_deinit(f);
    }

    c7_deque_clear(forms);
}
