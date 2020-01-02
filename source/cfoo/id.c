#include <ctype.h>
#include <stdlib.h>

#include "cfoo/id.h"
#include "cfoo/thread.h"
#include "codr7/rbtree.h"
#include "codr7/utils.h"

struct cf_id *cf_id_init(struct cf_id *id, const char *name) {
  id->name = c7_strdup(name);
  return id;
}

void cf_id_deinit(struct cf_id *id) {
  free(id->name);
}

bool cf_id_char(char c) {
  return isalpha(c);
}

struct cf_id *cf_id(struct cf_thread *t, const char *name) {
  struct cf_id *id = c7_rbtree_find(&t->ids, name);
  return id ? id : cf_id_init(c7_rbtree_add(&t->ids, name), name);
}

enum c7_order cf_id_compare(const void *key, const void *value) {
  return c7_compare_str(key, ((const struct cf_id *)value)->name);
}
