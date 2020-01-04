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

static const char *skip(const char *in, struct cf_point *point) {
  for (;;) {
    switch (*in) {
    case ' ':
      in++;
      point->column++;
      break;
    case '\n':
      in++;
      point->line++;
      point->column = CF_MIN_COLUMN;
    default:
      return in;
    }
  }
}

const char *cf_parse(struct cf_thread *thread,
		     const char *in,
		     struct cf_point *point,
		     struct c7_deque *out) {
  do {
    in = cf_parse_form(thread, skip(in, point), point, out);
  } while (*in && cf_ok(thread));

  return in;
}

const char *cf_parse_form(struct cf_thread *thread,
			   const char *in,
			   struct cf_point *point,
			   struct c7_deque *out) {
  char c = *in;
  
  switch (c) {
  case 0:
    return in;
  case '(':
    return cf_parse_params(thread, in, point, out);
  default:
    if (cf_id_char(c)) {
      return cf_parse_id(thread, in, point, out);
    }

    if (isdigit(c)) {
      return cf_parse_num(thread, in, point, out);
    }
  }
  
  cf_error(thread, *point, CF_ESYNTAX, "Unexpected char: %c (%d)", c, (int)c);
  return NULL;
}

const char *cf_parse_id(struct cf_thread *thread,
			const char *in,
			struct cf_point *point,
			struct c7_deque *out) {
  const char *start = in;

  while (*in && cf_id_char(*in)) {
    in++;
    point->column++;
  }

  size_t l = in - start;
  char name[l + 1];
  name[l] = 0;
  strncpy(name, start, l);

  cf_form_init(c7_deque_push_back(out), CF_ID, thread)->as_id =
    cf_id(thread, name);
  
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

const char *cf_parse_num(struct cf_thread *thread,
			 const char *in,
			 struct cf_point *point,
			 struct c7_deque *out) {
  int64_t v = 0;
  int base = 10;
  
  if (*in == '0') {
    in++;
    point->column++;
    
    switch (*in) {
    case 'b':
      base = 2;
      in++;
      point->column++;
      break;
    case 'x':
      base = 16;
      in++;
      point->column++;
      break;
    default:
      break;
    }
  }
  
  char c = 0;
  
  while ((c = *in)) {
    if (c == ' ' || c == '\n' || c == '(' || c == ')') {
      break;
    }

    int dv = char_int(c, base);
    
    if (dv == -1) {
      cf_error(thread, *point, CF_ESYNTAX,
	       "Invalid numeric char: %c (%d)", c, (int)c);
      
      return NULL;
    }
    
    v = v * base + dv;
    in++;
    point->column++;
  }

  cf_value_init(&cf_form_init(c7_deque_push_back(out),
			      CF_VALUE,
			      thread)->as_value,
		thread->int64_type)->as_int64 = v;
  
  return in;
}

const char *cf_parse_params(struct cf_thread *thread,
			    const char *in,
			    struct cf_point *point,
			    struct c7_deque *out) {
  char c = *in;
  
  if (c != '(') {
    cf_error(thread, *point, CF_ESYNTAX, "Invalid params: %c (%d)", c, (int)c);
    return NULL;
  }

  in++;
  point->column++;
  struct cf_form *f = cf_form_init(c7_deque_push_back(out), CF_PARAMS, thread);
  
  do {
    in = skip(in, point);
    
    if (*in == ')') {
      point->column++;
      return ++in;
    }

    in = cf_parse_form(thread, in, point, &f->as_params);
  } while (*in && cf_ok(thread));
  
  cf_error(thread, *point, CF_ESYNTAX, "Open params");
  return NULL;
}
