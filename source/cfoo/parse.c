#include <ctype.h>
#include <stddef.h>
#include "cfoo/parse.h"
#include "cfoo/status.h"

struct cf_status *cf_parse(const char *in,
			   struct cq_deque *out,
			   const char **end) {
  struct cf_status *s = NULL;
  while (*in && (s = cf_parse_token(in, out, end)) == cf_ok());
  return s;
}

struct cf_status *cf_parse_token(const char *in,
				 struct cq_deque *out,
				 const char **end) {
  char c = *in;

  switch (c) {
  case 0:
    if (end) {
      *end = in;
    }
    
    return cf_ok();
  default:
    if (isalpha(c)) {
      return cf_parse_id(in, out, end);
    }
  }
  
  return cf_error(CF_INVALID_INPUT, "Invalid input: %c (%d)", c, (int)c);
}

struct cf_status *cf_parse_id(const char *in,
			      struct cq_deque *out,
			      const char **end) {
  return cf_ok();
}

