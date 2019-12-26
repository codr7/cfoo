#ifndef CFOO_STATUS_H
#define CFOO_STATUS_H

#include <inttypes.h>
#include "cfoo/thread.h"

#define cf_error(thread, code, spec, ...)				\
  _cf_error(thread,							\
	    __FILE__, __LINE__,						\
	    code,							\
	    "Error in %s, line %" PRId16 ", column %" PRId16 "\n" spec,	\
	    "n/a", -1, -1, ## __VA_ARGS__, NULL)

enum cf_status_code {CF_OK, CF_INVALID_INPUT};

struct cf_status {
  enum cf_status_code code;  
  char *error;
};

struct cf_status *cf_ok();

struct cf_status *_cf_error(struct cf_thread *thread,
			    const char *file, int line,
			    enum cf_status_code code,
			    const char *spec, ...);

#endif
