#ifndef TMUX_RIME_H
#define TMUX_RIME_H 1

#include <sys/cdefs.h>

#include "config.h"

#ifdef RIME_API_DEPRECATED
#include <rime_api_deprecated.h>
#else
#include <rime_api.h>
#endif

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
