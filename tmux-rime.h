#ifndef TMUX_RIME_H
#define TMUX_RIME_H 1
#include <rime_api.h>
#include <sys/cdefs.h>
__BEGIN_DECLS

typedef struct rime_ui_t {
  char *left;
  char *right;
  char *left_sep;
  char *right_sep;
  char *cursor;
  char *indices[10];
} RimeUI;

RimeTraits RimeGetTraits();
int RimeWidth(char *str);
void RimeLoop(RimeSessionId session_id, struct rime_ui_t ui, char quit,
              int (*feed_keys)(const char *),
              void (*callback)(char *, char *, char *, char *, char *, char *));

__END_DECLS
#endif /* tmux-rime.h */
