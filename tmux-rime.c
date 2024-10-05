#define _XOPEN_SOURCE
#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <locale.h>
#include <rime_api.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "tmux-rime.h"

#define DEFAULT_BUFFER_SIZE 1024

static int width(char *str) {
  wchar_t wc[DEFAULT_BUFFER_SIZE] = L"";
  size_t wc_len = mbstowcs(wc, str, strlen(str));
  return wcswidth(wc, wc_len);
}

static void draw_ui(struct rime_ui_t ui, RimeContext context,
                    void (*callback)(char *, char *, char *, char *, char *,
                                     char *)) {
  char left_padding[DEFAULT_BUFFER_SIZE] = "";
  memset(left_padding, ' ', width(ui.left));
  char right[DEFAULT_BUFFER_SIZE] = "";
  char left[DEFAULT_BUFFER_SIZE] = "";

  strncpy(left, context.composition.preedit, context.composition.cursor_pos);
  strncpy(right, context.composition.preedit + context.composition.cursor_pos,
          context.composition.length - context.composition.cursor_pos);
  char str[DEFAULT_BUFFER_SIZE] = "";
  char *p = str;
  for (int index = 0; index < context.menu.num_candidates; index++) {
    if (context.menu.highlighted_candidate_index == index) {
      strcpy(p, ui.left_sep);
      p += strlen(ui.left_sep);
    } else if (context.menu.highlighted_candidate_index + 1 == index) {
      strcpy(p, ui.right_sep);
      p += strlen(ui.right_sep);
    } else {
      int n = width(ui.left_sep);
      memset(p, ' ', n);
      p += n;
    }
    strcpy(p, ui.indices[index]);
    p += strlen(ui.indices[index]);
    *(p++) = ' ';
    strcpy(p, context.menu.candidates[index].text);
    p += strlen(context.menu.candidates[index].text);
    if (context.menu.candidates[index].comment) {
      *(p++) = ' ';
      strcpy(p, context.menu.candidates[index].comment);
      p += strlen(context.menu.candidates[index].comment);
    }
  }
  if (context.menu.num_candidates ==
      context.menu.highlighted_candidate_index + 1) {
    strcpy(p, ui.right_sep);
    p += strlen(ui.right_sep);
  } else {
    int n = width(ui.right_sep);
    memset(p, ' ', n);
    p += n;
  }
  if (context.menu.is_last_page == False && context.menu.num_candidates > 0) {
    strcpy(p, ui.right);
    p += width(ui.right);
  }
  callback(left_padding, left, right,
           context.menu.page_no ? ui.left : left_padding, str, ui.cursor);
}

static int translate(int *c) {
  int mask = 0;
  if (!isprint(*c)) {
    if (*c == '\r') {
      *c = 65293;
    } else if (*c == '\x7f' || *c == '\b') {
      *c = 65288;
    } else if (*c == '\0') {
      mask += 1 << 2;
      *c = ' ';
    } else if (*c == '\x5f') {
      mask += 1 << 2;
      *c = '-';
    } else if (*c == '\x5e') {
      mask += 1 << 2;
      *c = '6';
    } else if (*c == '\t') {
      *c = 65289;
    } else {
      mask += 1 << 2;
      *c ^= 0x40;
      if (isupper(*c)) {
        *c = tolower(*c);
      }
    }
  }
  return mask;
}

static bool process_key(RimeSessionId session_id, int c, int mask,
                        struct rime_ui_t ui, int (*feed_keys)(char *),
                        void (*callback)(char *, char *, char *, char *, char *,
                                         char *)) {
  bool menu_is_empty = true;
  if (!RimeProcessKey(session_id, c, mask)) {
    if (mask == 0) {
      char src[2] = {c};
      feed_keys(src);
    }
    return menu_is_empty;
  }
  RIME_STRUCT(RimeContext, context);
  if (!RimeGetContext(session_id, &context)) {
    fputs("cannot get context", stderr);
    return menu_is_empty;
  }
  if (context.menu.num_candidates == 0) {
    RIME_STRUCT(RimeCommit, commit);
    if (RimeCommitComposition(session_id)) {
      if (!RimeGetCommit(session_id, &commit)) {
        fputs("cannot get commit", stderr);
        return menu_is_empty;
      }
      feed_keys(commit.text);
    }
    if (!RimeFreeCommit(&commit)) {
      fputs("cannot free commit", stderr);
      return menu_is_empty;
    }
  } else
    menu_is_empty = false;
  draw_ui(ui, context, callback);
  if (!RimeFreeContext(&context)) {
    fputs("cannot free context", stderr);
    return menu_is_empty;
  }
  return menu_is_empty;
}

/**
 * @param feed_keys handle output
 * @param callback draw UI
 */
void RimeLoop(RimeTraits traits, struct rime_ui_t ui, char quit,
              int (*feed_keys)(char *),
              void (*callback)(char *, char *, char *, char *, char *,
                               char *)) {
  setlocale(LC_CTYPE, "");
  RimeSetup(&traits);
  RimeInitialize(&traits);
  RimeSessionId session_id = RimeCreateSession();
  if (session_id == 0)
    err(errno, "cannot create session");

  bool menu_is_empty = true;
  while (true) {
    int mask = 0;
    int c = getchar();
    if (c == quit)
      break;
    if (c == '\e') {
      mask += 1 << 3;
      char src[3] = {c, getchar()};
      c = src[1];
      if (menu_is_empty && !isprint(c)) {
        feed_keys(src);
        continue;
      }
    }
    if (menu_is_empty && !isprint(c)) {
      char src[2] = {c};
      feed_keys(src);
      continue;
    }
    mask += translate(&c);
    menu_is_empty = process_key(session_id, c, mask, ui, feed_keys, callback);
  }

  if (RimeDestroySession(session_id) == False)
    err(errno, "cannot destroy session");
}
