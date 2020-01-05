#include <stdlib.h>

#include "cfoo/binding.h"
#include "cfoo/code.h"
#include "cfoo/error.h"
#include "cfoo/form.h"
#include "cfoo/method.h"
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

static void id_compile(struct cf_value *value,
		       struct cf_point point,
		       struct cf_code *out) {
  if (value->type == out->thread->method_type) {
    cf_op_init(c7_deque_push_back(&out->ops), CF_CALL)->as_call =
      (struct cf_call_op){.method = cf_method_ref(value->as_method),
			  .point = point};  
  } else {
    abort();
  }
}

void cf_compile(struct c7_deque *in,
		struct c7_rbtree *bindings,
		struct cf_code *out) {
  while (in->count) {
    struct cf_form *f = c7_deque_front(in);

    switch (f->type) {
    case CF_ID: {
      struct cf_binding *b = c7_rbtree_find(bindings, f->as_id);
      
      if (b) {
	id_compile(&b->value, f->point, out);
      } else {
	cf_error(out->thread, f->point,
		 CF_EUNKNOWN,
		 "Unknown id: %s", f->as_id->name);
      }
      
      cf_form_deinit(f);
      c7_deque_pop_front(in);
      break;
    }
    default:
      abort();
    }
  }
}

bool cf_eval(struct cf_code *in) {
  c7_deque_do(&in->ops, op) {
    if (!cf_op_eval(op, in->thread)) {
      return false;
    }
  }

  return true;
}
