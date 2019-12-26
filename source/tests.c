#include <assert.h>
#include <libceque/deque.h>
#include "cfoo/parse.h"
#include "cfoo/status.h"
#include "cfoo/token.h"

int main() {
  struct cf_thread *t = cf_thread_new();
  t->debug = true;
  
  struct cf_status *s = NULL;
  struct cq_deque out;
  cq_deque_init(&out, cf_tokens());
  s = cf_parse(t, "foo bar baz 42", &out, NULL);
  assert(s == cf_ok());
  return 0;
}
