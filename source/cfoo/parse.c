#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "cfoo/error.h"
#include "cfoo/form.h"
#include "cfoo/id.h"
#include "cfoo/parse.h"
#include "cfoo/point.h"
#include "codr7/deque.h"

const char *cf_parse(struct cf_thread *t,
		     struct cf_point *p,
		     const char *in,
		     struct c7_deque *out) {
  do {
    in = cf_parse_form(t, p, in, out);
  } while (*in && cf_ok(t));

  return in;
}

const char *cf_parse_form(struct cf_thread *t,
			   struct cf_point *p,
			   const char *in,
			   struct c7_deque *out) {
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
    if (cf_id_char(c)) {
      return cf_parse_id(t, p, in, out);
    }
  }
  
  cf_error(t, CF_INVALID_INPUT, "Invalid input: %c (%d)", c, (int)c);
  return NULL;
}

const char *cf_parse_id(struct cf_thread *t,
			struct cf_point *p,
			const char *in,
			struct c7_deque *out) {
  const char *start = in;

  while (*in && cf_id_char(*in)) {
    in++;
    p->column++;
  }

  size_t l = in - start;
  char name[l + 1];
  name[l] = 0;
  strncpy(name, start, l);
  cf_form_init(c7_deque_push_back(out), CF_ID)->as_id = cf_id(t, name);
  return in;
}
