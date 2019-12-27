#ifndef CFOO_FORM_H
#define CFOO_FORM_H

struct cf_id;

struct cf_form {
  union {
    struct cf_id *as_id;
  };
};

#endif
