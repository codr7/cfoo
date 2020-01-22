#include <stddef.h>
#include "cfoo/point.h"

struct cf_point *cf_point_init(struct cf_point *point,
			       const struct cf_id *file,
			       int16_t line, int16_t column) {
  point->file = file;
  point->line = line;
  point->column = column;
  return point;
}
