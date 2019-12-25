#ifndef CFOO_STATUS_H
#define CFOO_STATUS_H

enum cf_status_code {CF_OK, CF_INVALID_INPUT};

struct cf_status {
  enum cf_status_code code;
  char *error;
};

struct cf_status *cf_ok();

#endif
