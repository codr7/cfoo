#include <stddef.h>
#include <stdio.h>
#include "cfoo/error.h"
#include "cfoo/id.h"
#include "cfoo/parse.h"
#include "cfoo/point.h"

const char *cf_parse(struct cf_thread *t,
		     struct cf_point *p,
		     const char *in,
		     struct cq_deque *out) {
  do {
    in = cf_parse_form(t, p, in, out);
  } while (*in && cf_ok(t));

  return in;
}

const char *cf_parse_form(struct cf_thread *t,
			   struct cf_point *p,
			   const char *in,
			   struct cq_deque *out) {
  char c = *in;
  
  switch (c) {
  case 0:
    return in;
  case ' ':
    while (*in == ' ' || *in == '\t') { 
      in++;
      p->column++;
    }
    
    return cf_parse_form(t, p, in, out);
  case '\n':
    p->line++;
    p->column = CF_MIN_COLUMN;
  default:
    if (cf_is_id(c)) {
      return cf_parse_id(t, p, in, out);
    }
  }
  
  cf_error(t, CF_INVALID_INPUT, "Invalid input: %c (%d)", c, (int)c);
  return NULL;
}

const char *cf_parse_id(struct cf_thread *t,
			struct cf_point *p,
			const char *in,
			struct cq_deque *out) {
  //const char *start = in;

  while (*in && cf_is_id(*in)) {
    in++;
    p->column++;
  }

  return in;
}
