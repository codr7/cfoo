#include <errno.h>
#include <stdlib.h>

#include "cfoo/binding.h"
#include "cfoo/config.h"
#include "cfoo/error.h"
#include "cfoo/form.h"
#include "cfoo/id.h"
#include "cfoo/method.h"
#include "cfoo/method_set.h"
#include "cfoo/op.h"
#include "cfoo/thread.h"
#include "cfoo/type.h"
#include "cfoo/type_parent.h"

static enum c7_order compare_binding(const void *key, const void *value) {
  return cf_id_compare(key, ((const struct cf_binding *)value)->id);
}

static enum c7_order compare_id(const void *key, const void *value) {
  return c7_strcmp(key, ((const struct cf_id *)value)->name);
}

static enum c7_order compare_method(const void *key, const void *value) {
  return cf_id_compare(key, ((const struct cf_method *)value)->id);
}

static enum c7_order compare_method_set(const void *key, const void *value) {
  return cf_id_compare(key, ((const struct cf_method_set *)value)->id);
}

static enum c7_order compare_type(const void *key, const void *value) {
  return cf_id_compare(key, ((const struct cf_type *)value)->id);
}

static enum c7_order bool_compare(const struct cf_value *x, const struct cf_value *y) {
  return c7_compare(x->as_bool, y->as_bool);
}

static void bool_copy(struct cf_value *dst, struct cf_value *src) {
  dst->as_bool = src->as_bool;
}

static bool bool_dump(struct cf_thread *thread,
		       const struct cf_point *point,
		       const struct cf_value *v,
		       FILE *out) {
  printf("%c", v->as_bool ? 'T' : 'F');
  return true;
}

static bool bool_is(const struct cf_value *x, const struct cf_value *y) {
  return x->as_bool == y->as_bool;
}

static struct cf_type *add_bool_type(struct cf_thread *thread) {
  struct cf_type *t = cf_add_type(thread, cf_id(thread, "Bool"), thread->a_type);
  t->copy_value = bool_copy;
  t->compare_value = bool_compare;
  t->dump_value = bool_dump;
  t->is_value = bool_is;
  return t;
}

static enum c7_order int64_compare(const struct cf_value *x, const struct cf_value *y) {
  return c7_compare(x->as_int64, y->as_int64);
}

static void int64_copy(struct cf_value *dst, struct cf_value *src) {
  dst->as_int64 = src->as_int64;
}

static bool int64_dump(struct cf_thread *thread,
		       const struct cf_point *point,
		       const struct cf_value *v,
		       FILE *out) {
  printf("%" PRId64, v->as_int64);
  return true;
}

static bool int64_is(const struct cf_value *x, const struct cf_value *y) {
  return x->as_int64 == y->as_int64;
}

static struct cf_type *add_int64_type(struct cf_thread *thread) {
  struct cf_type *t = cf_add_type(thread, cf_id(thread, "Int64"), thread->a_type);
  t->copy_value = int64_copy;
  t->compare_value = int64_compare;
  t->dump_value = int64_dump;
  t->is_value = int64_is;
  return t;
}

static enum c7_order meta_compare(const struct cf_value *x, const struct cf_value *y) {
  return c7_compare(x->as_meta, y->as_meta);
}

static void meta_copy(struct cf_value *dst, struct cf_value *src) {
  dst->as_meta = cf_type_ref(src->as_meta);
}

static void meta_deinit(struct cf_value *v) {
  cf_type_deref(v->as_meta);
}

static bool meta_dump(struct cf_thread *thread,
		      const struct cf_point *point,
		      const struct cf_value *v,
		      FILE *out) {
  printf("Type(%s)", v->as_meta->id->name);
  return true;
}

static bool meta_is(const struct cf_value *x, const struct cf_value *y) {
  return x->as_meta == y->as_meta;
}

static struct cf_type *add_meta_type(struct cf_thread *thread) {
  struct cf_type *t = cf_add_type(thread, cf_id(thread, "Meta"));
  t->compare_value = meta_compare;
  t->copy_value = meta_copy;
  t->deinit_value = meta_deinit;
  t->dump_value = meta_dump;
  t->is_value = meta_is;
  return t;
}

static enum c7_order method_compare(const struct cf_value *x, const struct cf_value *y) {
  return c7_compare(x->as_method, y->as_method);
}

static void method_copy(struct cf_value *dst, struct cf_value *src) {
  dst->as_method = cf_method_ref(src->as_method);
}

static void method_deinit(struct cf_value *v) {
  cf_method_deref(v->as_method);
}

static bool method_dump(struct cf_thread *thread,
		        const struct cf_point *point,
		        const struct cf_value *v,
			FILE *out) {
  printf("Method(%s)", v->as_method->id->name);
  return true;
}

static bool method_is(const struct cf_value *x, const struct cf_value *y) {
  return x->as_method == y->as_method;
}

static struct cf_type *add_method_type(struct cf_thread *thread) {
  struct cf_type *t = cf_add_type(thread, cf_id(thread, "Method"), thread->a_type);
  t->compare_value = method_compare;
  t->copy_value = method_copy;
  t->deinit_value = method_deinit;
  t->dump_value = method_dump;
  t->is_value = method_is;
  return t;
}

static enum c7_order method_set_compare(const struct cf_value *x, const struct cf_value *y) {
  return c7_compare(x->as_method_set, y->as_method_set);
}

static void method_set_copy(struct cf_value *dst, struct cf_value *src) {}

static void method_set_deinit(struct cf_value *v) {}

static bool method_set_dump(struct cf_thread *thread,
		        const struct cf_point *point,
		        const struct cf_value *v,
			FILE *out) {
  printf("MethodSet(%s)", v->as_method_set->id->name);
  return true;
}

static bool method_set_is(const struct cf_value *x, const struct cf_value *y) {
  return x->as_method_set == y->as_method_set;
}

static struct cf_type *add_method_set_type(struct cf_thread *thread) {
  struct cf_type *t = cf_add_type(thread, cf_id(thread, "MethodSet"), thread->a_type);
  t->compare_value = method_set_compare;
  t->copy_value = method_set_copy;
  t->deinit_value = method_set_deinit;
  t->dump_value = method_set_dump;
  t->is_value = method_set_is;
  return t;
}

static enum c7_order time_compare(const struct cf_value *x, const struct cf_value *y) {
  const struct timespec *xt = &x->as_time, *yt = &y->as_time;  
  enum c7_order sec = c7_compare(xt->tv_sec, yt->tv_sec);
  return (sec == C7_EQ) ? c7_compare(xt->tv_nsec, yt->tv_nsec) : sec;
}

static void time_copy(struct cf_value *dst, struct cf_value *src) {
  struct timespec *dt = &dst->as_time, *st = &src->as_time;  
  dt->tv_sec = st->tv_sec;
  dt->tv_nsec = st->tv_nsec;
}

static bool time_dump(struct cf_thread *thread,
		      const struct cf_point *point,
		      const struct cf_value *v,
		      FILE *out) {
  struct tm *t = gmtime(&v->as_time.tv_sec);
  
  if (!t) {
    cf_error(thread, point, CF_ERUN, "Failed converting time: %d", errno);
    return false;
  }

  char buf[27];

  if (!strftime(buf, sizeof(buf), "Time(%Y-%m-%d %H:%M:%S)", t)) {
    cf_error(thread, point, CF_ERUN, "Failed formatting time: %d", errno);
    return false;
  }

  fputs(buf, out);
  return true;
}

static bool time_is(const struct cf_value *x, const struct cf_value *y) {
  const struct timespec *xt = &x->as_time, *yt = &y->as_time;
  return xt->tv_sec == yt->tv_sec && xt->tv_nsec == yt->tv_nsec;
}

static struct cf_type *add_time_type(struct cf_thread *thread) {
  struct cf_type *t = cf_add_type(thread, cf_id(thread, "Time"), thread->a_type);
  t->compare_value = time_compare;
  t->copy_value = time_copy;
  t->dump_value = time_dump;
  t->is_value = time_is;
  return t;
}

static bool is_imp(struct cf_thread *thread, const struct cf_point *point) {
  struct cf_value x = *cf_pop(thread), y = *cf_pop(thread);
  cf_value_init(cf_push(thread), thread->bool_type)->as_bool = cf_is(&x, &y);
  cf_value_deinit(&x);
  cf_value_deinit(&y);
  return true;
}

static bool debug_imp(struct cf_thread *thread, const struct cf_point *point) {
  thread->debug = !thread->debug;
  
  printf("Debug %s in %s, line %" PRId16 ", column %" PRId16 "\n",
	 thread->debug ? "enabled" : "disabled",
	 point->file->name, point->line, point->column);

  return true;
}

static bool now_imp(struct cf_thread *thread, const struct cf_point *point) {
  if (!timespec_get(&cf_value_init(cf_push(thread), thread->time_type)->as_time,
		    TIME_UTC)) {
    cf_error(thread, point, CF_ERUN, "Failed getting time: %d", errno);
    return false;
  }

  return true;
}

struct cf_thread *cf_thread_new() {
  struct cf_thread *t = malloc(sizeof(struct cf_thread));
  t->debug = false;

  c7_deque_pool_init(&t->error_pool, CF_SLAB_SIZE, sizeof(struct cf_error));
  c7_deque_init(&t->errors, &t->error_pool);

  c7_deque_pool_init(&t->form_pool, CF_SLAB_SIZE, sizeof(struct cf_form));

  c7_tree_pool_init(&t->id_pool, CF_SLAB_SIZE, sizeof(struct cf_id));
  c7_tree_init(&t->ids, compare_id, &t->id_pool);

  c7_tree_pool_init(&t->type_pool, CF_SLAB_SIZE, sizeof(struct cf_type));
  c7_tree_pool_init(&t->type_parent_pool, CF_SLAB_SIZE, sizeof(struct cf_type_parent));
  c7_tree_init(&t->types, compare_type, &t->type_pool);

  c7_tree_pool_init(&t->method_pool, CF_SLAB_SIZE, sizeof(struct cf_method));
  c7_tree_init(&t->methods, compare_method, &t->method_pool);

  c7_tree_pool_init(&t->method_set_pool, CF_SLAB_SIZE, sizeof(struct cf_method_set));
  c7_tree_init(&t->method_sets, compare_method_set, &t->method_set_pool);

  c7_tree_pool_init(&t->binding_pool, CF_SLAB_SIZE, sizeof(struct cf_binding));
  c7_tree_init(&t->bindings, compare_binding, &t->binding_pool);

  c7_deque_pool_init(&t->stack_pool, CF_SLAB_SIZE, sizeof(struct cf_value));
  c7_deque_init(&t->stack, &t->stack_pool);
  
  c7_deque_pool_init(&t->op_pool, CF_SLAB_SIZE, sizeof(struct cf_op));
  c7_chan_init(&t->chan, CF_SLAB_SIZE, sizeof(struct cf_value), 0);

  struct cf_point p;
  cf_point_init(&p, cf_id(t, "?"), -1, -1);
  
  t->next_type_tag = 0;
  t->meta_type = NULL;
  t->meta_type = add_meta_type(t);
  t->a_type = cf_add_type(t, cf_id(t, "A"));
  cf_derive(t->meta_type, t->a_type);
  t->bool_type = add_bool_type(t);
  t->method_type = add_method_type(t);
  t->method_set_type = add_method_set_type(t);
  t->int64_type = add_int64_type(t);
  t->time_type = add_time_type(t);

  cf_add_method(t, &p, &t->bindings, cf_id(t, "=="), 2, 1,
		cf_arg_type(cf_id(t, "x"), t->a_type), cf_arg_index(cf_id(t, "y"), 0),
		cf_ret_type(t->bool_type))->imp = is_imp;

  cf_add_method(t, &p, &t->bindings, cf_id(t, "debug"), 0, 0)->imp = debug_imp;
  cf_add_method(t, &p, &t->bindings, cf_id(t, "now"), 0, 1, cf_ret_type(t->time_type))->imp = now_imp;
  
  return t;
}

void cf_thread_free(struct cf_thread *thread) {
  c7_chan_deinit(&thread->chan);
  c7_deque_pool_deinit(&thread->op_pool);

  c7_deque_do(&thread->stack, v) {
    cf_value_deinit(v);
  }

  c7_deque_clear(&thread->stack);
  c7_deque_pool_deinit(&thread->stack_pool);

  c7_tree_do(&thread->bindings, b) {
    cf_binding_deinit(b);
  }
  
  c7_tree_clear(&thread->bindings);
  c7_tree_pool_deinit(&thread->binding_pool);

  c7_tree_do(&thread->methods, m) {
    cf_method_deinit(m);
  }
  
  c7_tree_clear(&thread->method_sets);
  c7_tree_pool_deinit(&thread->method_set_pool);
  
  c7_tree_clear(&thread->methods);
  c7_tree_pool_deinit(&thread->method_pool);

  c7_tree_do(&thread->types, t) {
    cf_type_deinit(t);
  }
  
  c7_tree_clear(&thread->types);
  c7_tree_pool_deinit(&thread->type_parent_pool);
  c7_tree_pool_deinit(&thread->type_pool);
  
  c7_tree_do(&thread->ids, id) {
    cf_id_deinit(id);
  }
  
  c7_tree_clear(&thread->ids);
  c7_tree_pool_deinit(&thread->id_pool);

  c7_deque_pool_deinit(&thread->form_pool);

  c7_deque_clear(&thread->errors);
  c7_deque_pool_deinit(&thread->error_pool);

  free(thread);
}

struct cf_value *cf_push(struct cf_thread *thread) {
  return c7_deque_push_back(&thread->stack);
}

struct cf_value *cf_pop(struct cf_thread *thread) {
  if (!thread->stack.count) {
    return NULL;
  }
  
  struct cf_value *v = c7_deque_back(&thread->stack);
  c7_deque_pop_back(&thread->stack);
  return v;
}

void cf_dump_stack(struct cf_thread *thread,
		   const struct cf_point *point,
		   FILE *out) {
  fputc('[', out);
  bool sep = false;
    
  c7_deque_do(&thread->stack, v) {
    if (sep) {
      fputc(' ', out);
    } else {
      sep = true;
    }
    
    if (!cf_dump(thread, point, v, out)) {
      fputc('?', out);
    }
  }
  
  fputc(']', out);
}
