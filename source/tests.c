#include <assert.h>
#include <libceque/deque.h>
#include "cfoo/parse.h"
#include "cfoo/status.h"
#include "cfoo/token.h"

int main() {
  struct cf_status *s = NULL;
  struct cq_deque out;
  cq_deque_init(&out, cf_tokens()); 
  s = cf_parse("foo bar baz", &out, NULL);
  assert(s == cf_ok());
  return 0;
}
