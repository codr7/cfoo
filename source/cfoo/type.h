#ifndef CFOO_TYPE_H
#define CFOO_TYPE_H

struct cf_id;

struct cf_type {
  struct cf_id *id;
};

struct cf_type *cf_type_init(struct cf_type *type, struct cf_id *id);
void cf_type_deinit(struct cf_type *type);

enum c7_order cf_type_compare(const void *key, const void *value);

#endif
