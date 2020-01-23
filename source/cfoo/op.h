#ifndef CFOO_OP_H
#define CFOO_OP_H

#include "cfoo/point.h"
#include "cfoo/value.h"

struct cf_method;

enum cf_op_type {CF_OCALL, CF_ODISPATCH, CF_ODROP, CF_ONOT, CF_OPUSH};

struct cf_call_op {
  struct cf_point point;
  struct cf_method *method;
};

struct cf_dispatch_op {
  struct cf_point point;
  struct cf_method_set *set;
};

struct cf_drop_op {
  struct cf_point point;
};

struct cf_not_op {
  struct cf_point point;
};

struct cf_push_op {
  struct cf_point point;
  struct cf_value value;
};

struct cf_op {
  enum cf_op_type type;
  
  union {
    struct cf_call_op as_call;
    struct cf_dispatch_op as_dispatch;
    struct cf_drop_op as_drop;
    struct cf_not_op as_not;
    struct cf_push_op as_push;
  };
};

struct cf_op *cf_op_init(struct cf_op *op, enum cf_op_type type);
void cf_op_deinit(struct cf_op *op);

bool cf_op_eval(struct cf_op *op, struct cf_thread *thread);

#endif
