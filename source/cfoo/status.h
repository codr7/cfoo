#ifndef CFOO_STATUS_H
#define CFOO_STATUS_H

#define cf_error(code, spec, ...)			\
  _cf_error(code,					\
	    "Error in %s, line %d: " spec,		\
	    __FILE__, __LINE__, ## __VA_ARGS__, NULL)

enum cf_status_code {CF_OK, CF_INVALID_INPUT};

struct cf_status {
  enum cf_status_code code;  
  char *error;
};

struct cf_status *cf_ok();
struct cf_status *_cf_error(enum cf_status_code code, const char *spec, ...);

#endif
