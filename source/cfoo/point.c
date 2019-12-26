#include <stddef.h>
#include "cfoo/point.h"

struct cf_point cf_point(const char *file, uint16_t line, uint16_t column) {
  return (struct cf_point){.file = NULL, .line = line, .column = column};
}
