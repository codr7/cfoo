#include <stdlib.h>

#include "cfoo/error.h"
#include "cfoo/method.h"
#include "cfoo/op.h"
#include "cfoo/thread.h"
#include "cfoo/type.h"

struct cf_op *cf_op_init(struct cf_op *op, enum cf_op_type type) {
  op->type = type;
  return op;
}

void cf_op_deinit(struct cf_op *op) {
  switch (op->type) {
  case CF_OCALL:
    cf_method_deref(op->as_call.method);
    break;
  case CF_OPUSH:
    cf_value_deinit(&op->as_push.value);
    break;
  default:
    break;
  }
}

static bool call_eval(struct cf_call_op *op) {
  return cf_call(op->method, &op->point);
}

static bool drop_eval(struct cf_drop_op *op, struct cf_thread *thread) {
  struct cf_value *v = cf_pop(thread);

  if (!v) {
      cf_error(thread, &op->point, CF_ERUN, "Nothing to drop");
      return false;
  }
  
  cf_value_deinit(v);
  return true;
  
}

static bool not_eval(struct cf_not_op *op, struct cf_thread *thread) {
  struct cf_value *v = cf_peek(thread);

  if (!v) {
    cf_error(thread, &op->point, CF_ERUN, "Nothing to negate");
    return false;
  }
  
  if (v->type != thread->bool_type) {
    cf_error(thread, &op->point, CF_ERUN, "Negation not supported: %s", v->type->id->name);
    return false;
  }
 
  v->as_bool = !v->as_bool;
  return true;
}

static bool push_eval(struct cf_push_op *op, struct cf_thread *thread) {
  cf_copy(cf_push(thread), &op->value);
  return true;
}

bool cf_op_eval(struct cf_op *op, struct cf_thread *thread) {
  switch(op->type) {
  case CF_OCALL:
    return call_eval(&op->as_call);
  case CF_ODROP:
    return drop_eval(&op->as_drop, thread);
  case CF_ONOT:
    return not_eval(&op->as_not, thread);
  case CF_OPUSH:
    return push_eval(&op->as_push, thread);
  default:
    abort();
  }
}

