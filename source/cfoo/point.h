#ifndef CFOO_POINT_H
#define CFOO_POINT_H

#include <inttypes.h>

#define CF_MIN_LINE 1
#define CF_MIN_COLUMN 0

struct cf_id;

struct cf_point {
  const struct cf_id *file;
  int16_t line, column;
};

struct cf_point cf_point(const struct cf_id *file,
			 int16_t line, int16_t column);

#endif
