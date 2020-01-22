#ifndef CFOO_METHOD_SET_H
#define CFOO_METHOD_SET_H

struct cf_method_set {
  const struct cf_id *id;
};

struct cf_method_set *cf_method_set_init(struct cf_method_set *set, const struct cf_id *id);
void cf_method_set_deinit(struct cf_method_set *set);

#endif
