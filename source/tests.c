#include <assert.h>
#include <stdio.h>

#include "cfoo/error.h"
#include "cfoo/form.h"
#include "cfoo/id.h"
#include "cfoo/parse.h"
#include "cfoo/point.h"
#include "cfoo/thread.h"
#include "cfoo/type.h"

static void parse_tests(struct cf_thread *t) {
  struct c7_deque out;
  c7_deque_init(&out, &t->form_pool);
  struct cf_point p;
  
  cf_point_init(&p, cf_id(t, "id test"), CF_MIN_LINE, CF_MIN_COLUMN);
  assert(!*cf_parse(t, "foo bar baz", &p, &out));
  assert(cf_ok(t));
  assert(out.count == 3);
  assert(p.line == CF_MIN_LINE && p.column == 11);
  cf_clear_forms(&out);

  cf_point_init(&p, cf_id(t, "int test"), CF_MIN_LINE, CF_MIN_COLUMN);
  assert(!*cf_parse(t, "42 0b101010 0x2a", &p, &out));
  assert(cf_ok(t));
  assert(out.count == 3);

  c7_deque_do(&out, _f) {
    struct cf_form *f = _f;
    assert(f->type == CF_VALUE);
    assert(f->as_value.as_int64 == 42);
  }

  assert(p.line == CF_MIN_LINE && p.column == 16);
  cf_clear_forms(&out);

  cf_point_init(&p, cf_id(t, "group test"), CF_MIN_LINE, CF_MIN_COLUMN);
  assert(!*cf_parse(t, "(foo bar baz)", &p, &out));
  assert(cf_ok(t));
  assert(out.count == 1);
  struct cf_form *f = c7_deque_back(&out);
  assert(f->type = CF_GROUP);
  assert(f->as_group.count == 3);
  assert(p.line == CF_MIN_LINE && p.column == 13);
  cf_clear_forms(&out);

  cf_point_init(&p, cf_id(t, "group test"), CF_MIN_LINE, CF_MIN_COLUMN);
  assert(!*cf_parse(t, "foo(bar baz)", &p, &out));
  assert(cf_ok(t));
  assert(out.count == 3);
  f = c7_deque_back(&out);
  assert(f->as_id == cf_id(t, "foo"));
  assert(p.line == CF_MIN_LINE && p.column == 12);
  cf_clear_forms(&out);
}

static void type_tests(struct cf_thread *thread) {
  assert(cf_find_root(thread->a_type, thread->a_type, true) == thread->a_type);
  assert(cf_find_root(thread->bool_type, thread->a_type, true) == thread->a_type);  

  assert(!cf_find_root(thread->a_type, thread->bool_type, true));  
  assert(cf_find_root(thread->a_type, thread->bool_type, false) == thread->a_type);  
}

int main() {
  struct cf_thread *t = cf_thread_new();
  t->debug = true;
  parse_tests(t);
  type_tests(t);
  cf_thread_free(t);
  return 0;
}
