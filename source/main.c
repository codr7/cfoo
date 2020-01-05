#include <codr7/stream.h>
#include <stdio.h>

#include "cfoo/config.h"
#include "cfoo/error.h"
#include "cfoo/id.h"
#include "cfoo/parse.h"
#include "cfoo/point.h"
#include "cfoo/thread.h"

static void repl(struct cf_thread *thread) {
  fprintf(stdout,
	  "cfoo v%d.%d.%d\n\n"
	  "Press Return in empty row to evaluate.\n\n",
	  CF_VERSION[0], CF_VERSION[1], CF_VERSION[2]);

  struct c7_stream in;
  c7_stream_init(&in);

  struct c7_deque forms;
  c7_deque_init(&forms, &thread->form_pool);
  
  for (;;) {
    printf("  ");
    char *l = c7_stream_getline(&in, stdin);
    
    if (!l || l[0] == '\n') {
      if (!l || in.length == 1) {
	break;
      }
      
      struct cf_point p = cf_point(cf_id(thread, "repl"), 
				   CF_MIN_LINE, CF_MIN_COLUMN);
      cf_parse(thread, in.data, &p, &forms);

      if (!cf_ok(thread)) {
	c7_deque_do(&thread->errors, _e) {
	  struct cf_error *e = _e;
	  fputs(e->message, stdout);
	  fputs("\n\n", stdout);
	}

	c7_deque_clear(&thread->errors);
      }

      c7_deque_clear(&forms);
      c7_stream_reset(&in);
    }
  }

  c7_deque_clear(&forms);
  c7_stream_deinit(&in);
}

int main() {
  struct cf_thread *thread = cf_thread_new();

  repl(thread);
  
  if (thread->debug) {
    cf_thread_free(thread);
  }
  
  return 0;
}
