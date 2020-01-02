#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "cfoo/error.h"
#include "cfoo/thread.h"

struct cf_error *_cf_error(struct cf_thread *t,
			   const char *file, int line,
			   enum cf_error_code code,
			   const char *spec, ...) {
  va_list args;
  va_start(args, spec);

  if (t->debug) {
    fprintf(stderr, "Error in %s, line %d\n", file, line);
    vfprintf(stderr, spec, args);
    abort();
  }
  
  struct cf_error *e = c7_deque_push_back(&t->errors);
  e->code = code;
  va_list len_args;
  va_copy(len_args, args);
  int len = vsnprintf(NULL, 0, spec, len_args);
  va_end(len_args);
  
  if (len < 0) {
    fprintf(stderr, "Error in %s, line %d\n", file, line);
    fputs(spec, stderr);
    fputc('\n', stderr);
    abort();
  } else {
    len++;
    e->message = malloc(len);
    vsnprintf(e->message, len, spec, args);
  }
  
  va_end(args);
  return e;
}

bool cf_ok(struct cf_thread *t) {
  return !t->errors.count;
}
