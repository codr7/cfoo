#include <stdlib.h>

#include "cfoo/method.h"
#include "cfoo/op.h"

struct cf_op *cf_op_init(struct cf_op *op, enum cf_op_type type) {
  op->type = type;
  return op;
}

void cf_op_deinit(struct cf_op *op) {
  switch (op->type) {
  case CF_CALL:
    cf_method_deref(op->as_call.method);
    break;
  default:
    break;
  }
}

bool call_eval(struct cf_call_op *op) {
  return cf_call(op->method, op->point);
}

bool cf_op_eval(struct cf_op *op, struct cf_thread *thread) {
  switch(op->type) {
  case CF_CALL:
    return call_eval(&op->as_call);
    break;
  default:
    abort();
  }
}

