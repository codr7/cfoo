#include <codr7/stream.h>
#include <codr7/tree.h>

#include "cfoo/binding.h"
#include "cfoo/id.h"
#include "cfoo/method.h"
#include "cfoo/method_set.h"
#include "cfoo/thread.h"
#include "cfoo/value.h"

struct cf_method *cf_method_init(struct cf_method *method,
				 struct cf_thread *thread,
				 const struct cf_id *id,
				 uint8_t arg_count, struct cf_arg *args,
				 uint8_t ret_count, struct cf_ret *rets) {
  method->thread = thread;
  method->id = id;
  method->arg_count = arg_count;
  method->ret_count = ret_count;
  method->imp = NULL;
  method->ref_count = 1;

  for (uint8_t i = 0; i < arg_count; i++) {
    method->args[i] = args[i];
  }

  for (uint8_t i = 0; i < ret_count; i++) {
    method->rets[i] = rets[i];
  }

  return method;
}

void cf_method_deinit(struct cf_method *method) {
  c7_list_remove(&method->set);
  
  for (uint8_t i = 0; i < method->arg_count; i++) {
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
				struct cf_method_set *set,
				uint8_t arg_count, struct cf_arg *args,
				uint8_t ret_count, struct cf_ret *rets) {
  struct c7_stream id_buf;
  c7_stream_init(&id_buf);
  c7_stream_puts(&id_buf, set->id->name);
  c7_stream_putc(&id_buf, '[');
  
  for (uint8_t i = 0; i < arg_count; i++) {
    if (i) {
      c7_stream_putc(&id_buf, ' ');
    }
    
    c7_stream_puts(&id_buf, args[i].id->name);
  }

  c7_stream_putc(&id_buf, ']');
  const struct cf_id *id = cf_id(thread, id_buf.data);
  c7_stream_deinit(&id_buf);
  
  struct cf_method *m = cf_method_init(c7_tree_add(&thread->methods, id),
				       thread, id, arg_count, args, ret_count, rets);

  cf_method_set_add(set, m);
  return m;
}

struct cf_method *cf_bind_method(struct cf_thread *thread,
				 const struct cf_id *id,
				 uint8_t arg_count, uint8_t ret_count,
				 ...) {
  struct cf_value *set = cf_find(thread, id);

  if (!set) {
    set = cf_bind(thread, id, thread->method_set_type);
    set->as_method_set = cf_method_set_init(c7_tree_add(&thread->method_sets, id), id);
  }

  va_list args;
  va_start(args, ret_count);
  struct cf_arg m_args[CF_MAX_ARGS];
  
  for (uint8_t i = 0; i < arg_count; i++) {
    m_args[i] = va_arg(args, struct cf_arg);
  }

  struct cf_ret m_rets[CF_MAX_RETS];

  for (uint8_t i = 0; i < ret_count; i++) {
    m_rets[i] = va_arg(args, struct cf_ret);
  }

  va_end(args);
  
  struct cf_method *m = cf_add_method(thread,
				      set->as_method_set,
				      arg_count, m_args,
				      ret_count, m_rets);

  cf_bind(thread, m->id, thread->method_type)->as_method = cf_method_ref(m);
  return m;
}

bool cf_call(struct cf_method *method, const struct cf_point *point) {
  return method->imp(method->thread, point);
}
