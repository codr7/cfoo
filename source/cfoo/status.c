#include <stdbool.h>
#include <stddef.h>
#include "cfoo/status.h"

struct cf_status *cf_ok() {
  static __thread struct cf_status s;
  static __thread bool init = true;

  if (init) {
    s.code = CF_OK;
    s.error = NULL;
    init = false;
  }

  return &s;
}
