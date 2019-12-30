#ifndef CFOO_ID_H
#define CFOO_ID_H

#include <stdbool.h>

struct cf_thread;

struct cf_id {
  char *name;
};

struct cf_id *cf_id_init(struct cf_id *id, const char *name);
void cf_id_deinit(struct cf_id *id);

bool cf_id_char(char c);
struct cf_id *cf_id(struct cf_thread *t, const char *name);
enum c7_order cf_id_compare(const void *key, const void *value);

#endif
