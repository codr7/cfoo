#include <assert.h>
#include "cfoo/error.h"
#include "cfoo/parse.h"
#include "cfoo/thread.h"
#include "cfoo/token.h"

int main() {
  struct cf_thread *t = cf_thread_new();
  t->debug = true;
  
  struct cq_deque out;
  cq_deque_init(&out, &t->token_pool);
  assert(!*cf_parse(t, "foo bar baz", &out) && cf_ok(t));
  return 0;
}
