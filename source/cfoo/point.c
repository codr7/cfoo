#include <stddef.h>
#include "cfoo/point.h"

struct cf_point cf_point(struct cf_id *file, uint16_t line, uint16_t column) {
  return (struct cf_point){.file = file, .line = line, .column = column};
}
