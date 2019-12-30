#ifndef CFOO_FORM_H
#define CFOO_FORM_H

struct cf_id;

enum cf_form_type {CF_ID};

struct cf_form {
  enum cf_form_type type;
  
  union {
    struct cf_id *as_id;
  };
};

struct cf_form *cf_form_init(struct cf_form *form, enum cf_form_type type);

#endif
