#ifndef CFOO_PARSE_H
#define CFOO_PARSE_H

struct cf_thread;
struct cq_deque;

struct cf_status *cf_parse(struct cf_thread *thread,
			   const char *in,
			   struct cq_deque *out,
			   const char **end);

struct cf_status *cf_parse_token(struct cf_thread *thread,
				 const char *in,
				 struct cq_deque *out,
				 const char **end);

struct cf_status *cf_parse_id(struct cf_thread *thread,
			      const char *in,
			      struct cq_deque *out,
			      const char **end);

#endif
