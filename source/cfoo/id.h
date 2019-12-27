#ifndef CFOO_ID_H
#define CFOO_ID_H

#include <stdbool.h>

struct cf_id {
  char *name;
};

bool cf_is_id(char c);

#endif
