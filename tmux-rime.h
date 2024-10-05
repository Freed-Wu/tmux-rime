#ifndef TMUX_RIME_H
#define TMUX_RIME_H 1
#include <rime_api.h>
#include <sys/cdefs.h>
__BEGIN_DECLS

struct UI {
  char *left;
  char *right;
  char *left_sep;
  char *right_sep;
  char *cursor;
  char *indices[10];
};

void rime_loop(RimeTraits traits, struct UI ui, char quit,
               int (*feed_keys)(char *),
               void (*callback)(char *, char *, char *, char *, char *,
                                char *));

__END_DECLS
#endif /* tmux-rime.h */
