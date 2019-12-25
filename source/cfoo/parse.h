#ifndef CFOO_PARSE_H
#define CFOO_PARSE_H

struct cq_deque;

struct cf_status *cf_parse(const char *in,
			   struct cq_deque *out,
			   const char **end);

struct cf_status *cf_parse_token(const char *in,
				 struct cq_deque *out,
				 const char **end);

struct cf_status *cf_parse_id(const char *in,
			      struct cq_deque *out,
			      const char **end);

#endif
