#include <stddef.h>
#include "cfoo/point.h"

struct cf_point cf_point(const struct cf_id *file,
			 int16_t line, int16_t column) {
  return (struct cf_point){.file = file, .line = line, .column = column};
}
