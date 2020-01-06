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
      break;
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
  } while (in && *in && cf_ok(thread));

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
    return cf_parse_group(thread, in, point, out);
  default:
    if (cf_id_char(c)) {
      return cf_parse_id(thread, in, point, out);
    }

    if (isdigit(c)) {
      return cf_parse_num(thread, in, point, out);
    }
  }
  
  cf_error(thread, point, CF_ESYNTAX, "Unexpected char: %c (%d)", c, (int)c);
  return NULL;
}

const char *cf_parse_group(struct cf_thread *thread,
			   const char *in,
			   struct cf_point *point,
			   struct c7_deque *out) {
  char c = *in;
  
  if (c != '(') {
    cf_error(thread, point, CF_ESYNTAX, "Invalid group: %c (%d)", c, (int)c);
    return NULL;
  }

  struct cf_form *f =
    cf_form_init(c7_deque_push_back(out), CF_GROUP, point, thread);

  in++;
  point->column++;

  do {
    in = skip(in, point);
    
    if (*in == ')') {
      point->column++;
      return ++in;
    }

    in = cf_parse_form(thread, in, point, &f->as_group);
  } while (*in && cf_ok(thread));
  
  cf_error(thread, point, CF_ESYNTAX, "Open group");
  return NULL;
}

const char *cf_parse_id(struct cf_thread *thread,
			const char *in,
			struct cf_point *point,
			struct c7_deque *out) {
  const char *start = in;
  struct cf_point start_point = *point;
  
  while (*in && cf_id_char(*in)) {
    in++;
    point->column++;
  }

  size_t l = in - start;
  char name[l + 1];
  name[l] = 0;
  strncpy(name, start, l);

  struct cf_form *f = (*in == '(' &&
		       (in = cf_parse_group(thread, in, point, out)))
    ? c7_deque_push_front(&((struct cf_form *)c7_deque_back(out))->as_group)
    : c7_deque_push_back(out);
    
  cf_form_init(f, CF_ID, &start_point, thread)->as_id = cf_id(thread, name);
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
  struct cf_point start_point = *point;
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
      cf_error(thread, point, CF_ESYNTAX,
	       "Invalid numeric char: %c (%d)", c, (int)c);
      
      return NULL;
    }
    
    v = v * base + dv;
    in++;
    point->column++;
  }

  cf_value_init(&cf_form_init(c7_deque_push_back(out),
			      CF_VALUE,
			      &start_point,
			      thread)->as_value,
		thread->int64_type)->as_int64 = v;
  
  return in;
}
