#ifndef CFOO_TOKEN_H
#define CFOO_TOKEN_H

struct cf_id;

struct cf_token {
  union {
    struct cf_id *as_id;
  };
};

#endif
