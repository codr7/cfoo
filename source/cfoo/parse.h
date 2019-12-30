#ifndef CFOO_PARSE_H
#define CFOO_PARSE_H

struct c7_deque;
struct cf_point;
struct cf_thread;

const char *cf_parse(struct cf_thread *t,
		     struct cf_point *p,
		     const char *in,
		     struct c7_deque *out);

const char *cf_parse_form(struct cf_thread *t,
			   struct cf_point *p,
			   const char *in,
			   struct c7_deque *out);

const char *cf_parse_id(struct cf_thread *t,
			struct cf_point *p,
			const char *in,
			struct c7_deque *out);

#endif
