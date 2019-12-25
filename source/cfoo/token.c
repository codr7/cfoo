#include <libceque/pool.h>
#include <stdbool.h>
#include "cfoo/token.h"

struct cq_pool *cf_tokens() {
  static __thread struct cq_pool p;
  static __thread bool init = true;

  if (init) {
    cq_pool_init(&p, CF_TOKEN_BLOCK_SIZE, sizeof(struct cf_token));
    init = false;
  }

  return &p;
}
