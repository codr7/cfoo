#include <ctype.h>
#include "cfoo/id.h"

bool cf_is_id(char c) {
  return isalpha(c);
}
