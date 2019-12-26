#ifndef CFOO_PARSE_H
#define CFOO_PARSE_H

struct cf_thread;
struct cq_deque;

const char *cf_parse(struct cf_thread *t, const char *in, struct cq_deque *out);

const char *cf_parse_token(struct cf_thread *t,
			   const char *in,
			   struct cq_deque *out);

const char *cf_parse_id(struct cf_thread *t,
			const char *in,
			struct cq_deque *out);

#endif
