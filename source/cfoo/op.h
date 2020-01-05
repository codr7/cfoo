#ifndef CFOO_OP_H
#define CFOO_OP_H

struct cf_method;

enum cf_op_type {CF_CALL};

struct cf_call_op {
  struct cf_method *method;
  struct cf_point point;
};
  
struct cf_op {
  enum cf_op_type type;
  
  union {
    struct cf_call_op as_call;
  };
};

struct cf_op *cf_op_init(struct cf_op *op, enum cf_op_type type);
void cf_op_deinit(struct cf_op *op);

bool cf_op_eval(struct cf_op *op, struct cf_thread *thread);

#endif
