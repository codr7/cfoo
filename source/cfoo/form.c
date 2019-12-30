#include "cfoo/form.h"

struct cf_form *cf_form_init(struct cf_form *form, enum cf_form_type type) {
  form->type = type;
  return form;
}
