#include <codr7/deque.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "cfoo/error.h"
#include "cfoo/form.h"
#include "cfoo/id.h"
#include "cfoo/parse.h"
#include "cfoo/point.h"
#include "cfoo/thread.h"
#include "cfoo/value.h"

static const char *skip(const char *in, struct cf_point *p) {
  for (;;) {
    switch (*in) {
    case ' ':
      in++;
      p->column++;
      break;
    case '\n':
      in++;
      p->line++;
      p->column = CF_MIN_COLUMN;
    default:
      return in;
    }
  }
}

const char *cf_parse(struct cf_thread *t,
		     const char *in,
		     struct cf_point *p,
		     struct c7_deque *out) {
  do {
    in = cf_parse_form(t, skip(in, p), p, out);
  } while (*in && cf_ok(t));

  return in;
}

const char *cf_parse_form(struct cf_thread *t,
			   const char *in,
			   struct cf_point *p,
			   struct c7_deque *out) {
  char c = *in;
  
  switch (c) {
  case 0:
    return in;
  case '(':
    return cf_parse_params(t, in, p, out);
  default:
    if (cf_id_char(c)) {
      return cf_parse_id(t, in, p, out);
    }

    if (isdigit(c)) {
      return cf_parse_num(t, in, p, out);
    }
  }
  
  cf_error(t, *p, CF_ESYNTAX, "Unexpected char: %c (%d)", c, (int)c);
  return NULL;
}

const char *cf_parse_id(struct cf_thread *t,
			const char *in,
			struct cf_point *p,
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
  cf_form_init(c7_deque_push_back(out), CF_ID, t)->as_id = cf_id(t, name);
  return in;
}

static int char_int(char c, int base) {
  if (isdigit(c)) {
    return c - '0';
  }

  if (base == 16 && c >= 'a' && c <= 'f') {
    return 10 + c - 'a';
  }

  return -1;
}

const char *cf_parse_num(struct cf_thread *t,
			 const char *in,
			 struct cf_point *p,
			 struct c7_deque *out) {
  int64_t v = 0;
  int base = 10;
  
  if (*in == '0') {
    in++;
    p->column++;
    
    switch (*in) {
    case 'b':
      base = 2;
      in++;
      p->column++;
      break;
    case 'x':
      base = 16;
      in++;
      p->column++;
      break;
    default:
      break;
    }
  }
  
  char c = 0;
  
  while ((c = *in)) {
    if (c == ' ' || c == '\t' || c == '\n' || c == '(' || c == ')') {
      break;
    }

    int dv = char_int(c, base);
    
    if (dv == -1) {
      cf_error(t, *p, CF_ESYNTAX, "Invalid numeric char: %c (%d)", c, (int)c);
      return NULL;
    }
    
    v = v * base + dv;
    in++;
    p->column++;
  }

  cf_value_init(&cf_form_init(c7_deque_push_back(out), CF_VALUE, t)->as_value,
		t->int64_type)->as_int64 = v;
  
  return in;
}

const char *cf_parse_params(struct cf_thread *t,
			    const char *in,
			    struct cf_point *p,
			    struct c7_deque *out) {
  char c = *in;
  
  if (c != '(') {
    cf_error(t, *p, CF_ESYNTAX, "Invalid params: %c (%d)", c, (int)c);
    return NULL;
  }

  in++;
  p->column++;
  struct cf_form *f = cf_form_init(c7_deque_push_back(out), CF_PARAMS, t);
  
  do {
    in = skip(in, p);
    
    if (*in == ')') {
      p->column++;
      return ++in;
    }

    in = cf_parse_form(t, in, p, &f->as_params);
  } while (*in && cf_ok(t));
  
  cf_error(t, *p, CF_ESYNTAX, "Open params");
  return NULL;
}
