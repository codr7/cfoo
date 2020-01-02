#include <assert.h>
#include <stdio.h>

#include "cfoo/error.h"
#include "cfoo/form.h"
#include "cfoo/parse.h"
#include "cfoo/point.h"
#include "cfoo/thread.h"

static void parse_tests(struct cf_thread *t) {
  struct c7_deque out;
  c7_deque_init(&out, &t->form_pool);
  struct cf_point p = cf_point("n/a", CF_MIN_LINE, CF_MIN_COLUMN);
  assert(cf_ok(t));
  assert(!*cf_parse(t, "foo bar baz", &p, &out));
  assert(out.count == 3);
  assert(p.line == CF_MIN_LINE && p.column == 11);
  cf_clear_forms(&out);

  p = cf_point("n/a", CF_MIN_LINE, CF_MIN_COLUMN);
  assert(!*cf_parse(t, "(foo bar baz)", &p, &out));
  assert(out.count == 1);
  struct cf_form *f = c7_deque_back(&out);
  assert(f->type = CF_PARAMS);
  assert(f->as_params.count == 3);
  assert(p.line == CF_MIN_LINE && p.column == 13);
  cf_clear_forms(&out);
}

int main() {
  struct cf_thread *t = cf_thread_new();
  t->debug = true;
  parse_tests(t);
  cf_thread_free(t);
  return 0;
}
