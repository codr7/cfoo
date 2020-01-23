#include <stdlib.h>

#include "cfoo/binding.h"
#include "cfoo/code.h"
#include "cfoo/error.h"
#include "cfoo/form.h"
#include "cfoo/method.h"
#include "cfoo/method_set.h"
#include "cfoo/op.h"
#include "cfoo/thread.h"
#include "cfoo/value.h"

struct cf_code *cf_code_init(struct cf_code *code, struct cf_thread *thread) {
  code->thread = thread;
  c7_deque_init(&code->ops, &thread->op_pool);
  return code;
}

void cf_code_deinit(struct cf_code *code) {
  cf_code_clear(code);
}

void cf_code_clear(struct cf_code *code) {
  c7_deque_do(&code->ops, op) {
    cf_op_deinit(op);
  }

  c7_deque_clear(&code->ops);
}

static void id_compile(const struct cf_point *point,
		       struct cf_value *value,
		       struct cf_code *out) {
  if (value->type == out->thread->method_type) {
    cf_op_init(c7_deque_push_back(&out->ops), CF_OCALL)->as_call =
      (struct cf_call_op){.point = *point, .method = cf_method_ref(value->as_method)};  
  } else if (value->type == out->thread->method_set_type) {
    struct cf_method_set *ms = value->as_method_set;

    if (ms->count == 1) {
      struct cf_method *m = c7_baseof(ms->items.next, struct cf_method, set_item);

      cf_op_init(c7_deque_push_back(&out->ops), CF_OCALL)->as_call =
	(struct cf_call_op){.point = *point, .method = cf_method_ref(m)};
    } else {
      cf_op_init(c7_deque_push_back(&out->ops), CF_ODISPATCH)->as_dispatch =
	(struct cf_dispatch_op){.point = *point, .set = ms};      
    }
  } else {
    struct cf_push_op *op = &cf_op_init(c7_deque_push_back(&out->ops), CF_OPUSH)->as_push;
    op->point = *point;
    cf_copy(&op->value, value);
  }
}

bool cf_compile(struct c7_deque *in,
		struct c7_tree *bindings,
		struct cf_code *out) {
  while (in->count) {
    struct cf_form *f = c7_deque_front(in);

    switch (f->type) {
    case CF_FDROP:
      cf_op_init(c7_deque_push_back(&out->ops), CF_ODROP)->as_drop =
	(struct cf_drop_op){.point = f->point};  
      
      cf_form_deinit(f);
      c7_deque_pop_front(in);
      break;
    case CF_FID: {
      struct cf_binding *b = c7_tree_find(bindings, f->as_id);
      
      if (b) {
	id_compile(&f->point, &b->value, out);
      } else {
	cf_error(out->thread, &f->point, CF_EUNKNOWN, "Unknown id: %s", f->as_id->name);
	return false;
      }
      
      cf_form_deinit(f);
      c7_deque_pop_front(in);
      break;
    }
    case CF_FNOT:
      cf_op_init(c7_deque_push_back(&out->ops), CF_ONOT)->as_not =
	(struct cf_not_op){.point = f->point};  
      
      cf_form_deinit(f);
      c7_deque_pop_front(in);
      break;        
    case CF_FVALUE: {
      struct cf_push_op *op = &cf_op_init(c7_deque_push_back(&out->ops), CF_OPUSH)->as_push;
      op->point = f->point;
      cf_copy(&op->value, &f->as_value);
      cf_form_deinit(f);
      c7_deque_pop_front(in);
      break;
    }
    default:
      cf_error(out->thread, &f->point, CF_ESYNTAX, "Unexpected form");
      return false;
    }
  }

  return true;
}

bool cf_eval(struct cf_code *in) {
  c7_deque_do(&in->ops, op) {
    if (!cf_op_eval(op, in->thread)) {
      return false;
    }
  }

  return true;
}

