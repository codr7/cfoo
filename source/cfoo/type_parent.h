#ifndef CFOO_TYPE_PARENT
#define CFOO_TYPE_PARENT

struct cf_type;

struct cf_type_parent {
  struct cf_type *root, *type;
};

void cf_type_parent_init(struct cf_type_parent *parent, struct cf_type *root, struct cf_type *type);
void cf_type_parent_deinit(struct cf_type_parent *parent);

#endif
