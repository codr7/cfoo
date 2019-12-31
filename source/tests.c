#include <assert.h>
#include "cfoo/error.h"
#include "cfoo/form.h"
#include "cfoo/parse.h"
#include "cfoo/point.h"
#include "cfoo/thread.h"

int main() {
  struct cf_thread *t = cf_thread_new();
  t->debug = true;
  
  struct c7_deque out;
  c7_deque_init(&out, &t->form_pool);
  struct cf_point p = cf_point("n/a", CF_MIN_LINE, CF_MIN_COLUMN);
  assert(cf_ok(t));
  assert(!*cf_parse(t, &p, "foo bar baz", &out));
  assert(out.count == 3);
  assert(p.line == CF_MIN_LINE && p.column == 11);
  c7_deque_clear(&out);
  cf_thread_free(t);
  return 0;
}
