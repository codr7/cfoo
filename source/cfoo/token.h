#ifndef CFOO_TOKEN_H
#define CFOO_TOKEN_H

#define CF_TOKEN_BLOCK_SIZE 32

struct cf_id;
struct cq_pool;

struct cf_token {
  union {
    struct cf_id *as_id;
  };
};

struct cq_pool *cf_tokens();

#endif
