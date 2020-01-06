#include <ctype.h>
#include <stdio.h>
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

enum c7_order cf_id_compare(const struct cf_id *x,
			    const struct cf_id *y) {
  return c7_strcmp(x->name, y->name);
}

bool cf_id_char(char c) {
  return isalpha(c);
}

const struct cf_id *cf_id(struct cf_thread *thread, const char *name) {
  struct cf_id *id = c7_rbtree_find(&thread->ids, name);
  return id ? id : cf_id_init(c7_rbtree_add(&thread->ids, name), name);
}
