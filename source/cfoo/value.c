#include "cfoo/id.h"
#include "cfoo/type.h"
#include "cfoo/value.h"

struct cf_value *cf_value_init(struct cf_value *value, struct cf_type *type) {
  value->type = type;
  return value;
}

void cf_value_deinit(struct cf_value *value) {
  if (value->type->deinit_value) {
    value->type->deinit_value(value);
  }
}

enum c7_order cf_compare(const struct cf_value *x,
			 const struct cf_value *y) {
  return (x->type == y->type)
    ? x->type->compare_value(x, y)
    : cf_id_compare(x->type->id, y->type->id);
}

void cf_copy(struct cf_value *dst, struct cf_value *src) {
  dst->type = src->type;
  src->type->copy_value(dst, src);
}

bool cf_dump(struct cf_thread *thread,
		   const struct cf_point *point,
		   const struct cf_value *v,
		   FILE *out) {
  return v->type->dump_value(thread, point, v, out);
}
  
bool cf_is(const struct cf_value *x, const struct cf_value *y) {
  return x->type == y->type && x->type->is_value(x, y);
}


