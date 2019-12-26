#include <stddef.h>
#include <stdio.h>
#include "cfoo/error.h"
#include "cfoo/id.h"
#include "cfoo/parse.h"

const char *cf_parse(struct cf_thread *t, const char *in, struct cq_deque *out) {
  do {
    in = cf_parse_token(t, in, out);
  } while (*in && cf_ok(t));

  return in;
}

const char *cf_parse_token(struct cf_thread *t,
			   const char *in,
			   struct cq_deque *out) {
  char c = *in;
  
  switch (c) {
  case 0:
    return in;
  case ' ':
    while (*in == ' ') { in++; }
    return cf_parse_token(t, in, out);
  default:
    if (cf_is_id(c)) {
      return cf_parse_id(t, in, out);
    }
  }
  
  cf_error(t, CF_INVALID_INPUT, "Invalid input: %c (%d)", c, (int)c);
  return NULL;
}

const char *cf_parse_id(struct cf_thread *t,
			const char *in,
			struct cq_deque *out) {
  //const char *start = in;

  while (*in && cf_is_id(*in)) {
    in++;
  }

  return in;
}
