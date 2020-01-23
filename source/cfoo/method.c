#include <codr7/stream.h>
#include <codr7/tree.h>
#include <stdlib.h>

#include "cfoo/binding.h"
#include "cfoo/error.h"
#include "cfoo/id.h"
#include "cfoo/method.h"
#include "cfoo/method_set.h"
#include "cfoo/thread.h"
#include "cfoo/type.h"
#include "cfoo/value.h"

struct cf_method *cf_method_init(struct cf_method *method,
				 struct cf_thread *thread,
				 const struct cf_point *point,
				 const struct cf_id *id,
				 struct cf_method_set *set,
				 struct cf_arg *args,
				 uint8_t ret_count, struct cf_ret *rets) {
  method->thread = thread;
  
  if (point) {
    method->point = *point;
  } else {
    cf_point_init(&method->point, cf_id(thread, "?"), -1, -1);
  }
  
  method->id = id;
  method->set = set;
  method->ret_count = ret_count;
  method->imp = NULL;
  method->ref_count = 1;

  for (uint8_t i = 0; i < set->arg_count; i++) {
    method->args[i] = args[i];
  }

  for (uint8_t i = 0; i < ret_count; i++) {
    method->rets[i] = rets[i];
  }

  cf_method_set_add(set, method);
  return method;
}

void cf_method_deinit(struct cf_method *method) {
  c7_list_remove(&method->set_item);
  
  for (uint8_t i = 0; i < method->set->arg_count; i++) {
    cf_arg_deinit(method->args + i);
  }

  for (uint8_t i = 0; i < method->ret_count; i++) {
    cf_ret_deinit(method->rets + i);
  }
}

struct cf_method *cf_method_ref(struct cf_method *method) {
  method->ref_count++;
  return method;
}

void cf_method_deref(struct cf_method *method) {
  if (!--method->ref_count) {
    cf_method_deinit(method);
    c7_tree_remove(&method->thread->methods, method->id);
  }
}

struct cf_method *cf_add_method(struct cf_thread *thread,
				const struct cf_point *point,
				struct cf_method_set *set,
				struct cf_arg *args,
				uint8_t ret_count, struct cf_ret *rets) {
  struct c7_stream id_buf;
  c7_stream_init(&id_buf);
  c7_stream_puts(&id_buf, set->id->name);
  c7_stream_putc(&id_buf, '[');
  
  for (uint8_t i = 0; i < set->arg_count; i++) {
    if (i) {
      c7_stream_putc(&id_buf, ' ');
    }

    struct cf_arg *a = args + i;
    
    switch (a->type) {
    case CF_AINDEX:
      c7_stream_printf(&id_buf, "%" PRIu8, a->as_index);
      break;
    case CF_ATYPE:
      c7_stream_puts(&id_buf, a->as_type->id->name);
      break;
    case CF_AVALUE:
      abort();
    }
  }

  c7_stream_putc(&id_buf, ']');
  const struct cf_id *id = cf_id(thread, id_buf.data);
  c7_stream_deinit(&id_buf);
  
  return cf_method_init(c7_tree_add(&thread->methods, id),
			thread, point, id, set, args, ret_count, rets);
}

struct cf_method *cf_bind_method(struct cf_thread *thread,
				 const struct cf_point *point,
				 const struct cf_id *id,
				 uint8_t arg_count, struct cf_arg *args,
				 uint8_t ret_count, struct cf_ret *rets) {	
  struct cf_value *set = cf_find(thread, id);

  if (set) {
    struct cf_method_set *ms = set->as_method_set;
    
    if (arg_count != ms->arg_count) {
      cf_error(thread, point, CF_ERUN,
	       "Wrong number of method arguments: %" PRIu8 "(%" PRIu8 ")",
	       arg_count, ms->arg_count);

      return NULL;
    }

    for (uint8_t i = 0; i < ms->arg_count; i++) {
      struct cf_arg *a = args + i;
      if (a->type == CF_AINDEX && a->as_index >= ms->arg_count) {
	cf_error(thread, point, CF_ERUN,
		 "Argument index out of bounds: %" PRIu8 " (%" PRIu8 ")",
		 a->as_index, ms->arg_count);
	
	return NULL;
      }
    }
  } else {
    set = cf_bind(thread, id, thread->method_set_type);
    set->as_method_set = cf_method_set_init(c7_tree_add(&thread->method_sets, id), id, arg_count);
  }

  struct cf_method *m = cf_add_method(thread,
				      point,
				      set->as_method_set,
				      args,
				      ret_count, rets);

  cf_bind(thread, m->id, thread->method_type)->as_method = cf_method_ref(m);
  return m;
}

bool cf_applicable(struct cf_method *method, struct cf_value *stack_pointer) {
    struct cf_arg *a = method->args;
    struct cf_value *s = stack_pointer;
    
    for (uint8_t i = 0; i < method->set->arg_count; i++, a++, s++) {
      if (a->type == CF_ATYPE && !cf_find_root(s->type, a->as_type, true)) {
	return false;
      }
      
      if (a->type == CF_AINDEX && !cf_find_root(s->type, stack_pointer[a->as_index].type, true)) {
	return false;
      }

      if (a->type == CF_AVALUE && cf_compare(s, &a->as_value) != C7_EQ) {
	return false;
      }
    }

    return true;
}

bool cf_call(struct cf_method *method, const struct cf_point *point) {
  return method->imp(method->thread, point);
}
