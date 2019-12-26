#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "cfoo/status.h"

struct cf_status *cf_ok() {
  static __thread struct cf_status s = {.code = CF_OK, .error = NULL};
  return &s;
}

struct cf_status *_cf_error(struct cf_thread *thread,
			    const char *file, int line,
			    enum cf_status_code code,
			    const char *spec, ...) {
  va_list args;
  va_start(args, spec);

  if (thread->debug) {
    fprintf(stderr, "Error in %s, line %d\n", file, line);
    vfprintf(stderr, spec, args);
    abort();
  }
  
  struct cf_status *e = malloc(sizeof(struct cf_status));
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
    e->error = malloc(len);
    vsnprintf(e->error, len, spec, args);
  }
  
  va_end(args);
  return e;
}
