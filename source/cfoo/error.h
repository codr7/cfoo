#ifndef CFOO_ERROR_H
#define CFOO_ERROR_H

#include <inttypes.h>
#include <stdbool.h>

#include "cfoo/id.h"

#define cf_error(thread, point, code, spec, ...) ({			\
  const struct cf_point *_point = point;				\
  _cf_error(thread,							\
	    __FILE__, __LINE__,						\
	    code,							\
	    "Error in %s, line %" PRId16 ", column %" PRId16 "\n"	\
	    spec,							\
	    _point ? _point->file->name : "?",				\
	    _point ? _point->line : -1,					\
	    _point ? _point->column : -1,				\
	    ## __VA_ARGS__, NULL);					\
  })

struct cf_thread;

enum cf_error_code {CF_ERUN, CF_ESYNTAX, CF_EUNKNOWN};

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
