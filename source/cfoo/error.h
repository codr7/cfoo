#ifndef CFOO_ERROR_H
#define CFOO_ERROR_H

#include <inttypes.h>
#include <stdbool.h>

#define cf_error(thread, point, code, spec, ...)			\
  _cf_error(thread,								\
	    __FILE__, __LINE__,						\
	    code,							\
	    "Error in %s, line %" PRId16 ", column %" PRId16 "\n" spec,	\
	    "n/a", (point).line, (point).column, ## __VA_ARGS__, NULL)

struct cf_thread;

enum cf_error_code {CF_ESYNTAX};

struct cf_error {
  enum cf_error_code code;  
  char *message;
};

struct cf_error *_cf_error(struct cf_thread *thread,
			   const char *file, int line,
			   enum cf_error_code code,
			   const char *spec, ...);

bool cf_ok(struct cf_thread *thread);

#endif
