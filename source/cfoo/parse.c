#include <stddef.h>
#include "cfoo/id.h"
#include "cfoo/parse.h"
#include "cfoo/status.h"

struct cf_status *cf_parse(const char *in,
			   struct cq_deque *out,
			   const char **end) {
  struct cf_status *s = NULL;
  const char *_end = in;
  while (*_end && (s = cf_parse_token(in, out, &_end)) == cf_ok());
  if (end) { *end = _end; }
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
  case ' ':
    while (*in == ' ') { in++; }
    return cf_parse_token(in, out, end);
  default:
    if (cf_is_id(c)) {
      return cf_parse_id(in, out, end);
    }
  }
  
  return cf_error(CF_INVALID_INPUT, "Invalid input: %c (%d)", c, (int)c);
}

struct cf_status *cf_parse_id(const char *in,
			      struct cq_deque *out,
			      const char **end) {
  //const char *start = in;

  while (*in && cf_is_id(*in)) {
    in++;
  }

  if (end) { *end = in; }
  return cf_ok();
}
