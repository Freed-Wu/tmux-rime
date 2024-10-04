#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <getopt.h>
#include <libgen.h>
#include <locale.h>
#include <rime_api.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#define __USE_XOPEN
#include <wchar.h>

#define VERSION "0.0.1"
#define DEFAULT_BUFFER_SIZE 1024

static char shortopts[] = "hVs:u:l:n:c:v:a:m:";
static struct option longopts[] = {
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'V'},
    {"shared_data_dir", required_argument, NULL, 's'},
    {"user_data_dir", required_argument, NULL, 'u'},
    {"log_dir", required_argument, NULL, 'o'},
    {"distribution_name", required_argument, NULL, 'n'},
    {"distribution_code_name", required_argument, NULL, 'c'},
    {"distribution_version", required_argument, NULL, 'v'},
    {"app_name", required_argument, NULL, 'a'},
    {"min_log_level", required_argument, NULL, 'm'},
    {"left", required_argument, NULL, 'L'},
    {"right", required_argument, NULL, 'R'},
    {"left_sep", required_argument, NULL, 'l'},
    {"right_sep", required_argument, NULL, 'r'},
    {"cursor", required_argument, NULL, 'I'},
    {"index1", required_argument, NULL, '1'},
    {"index2", required_argument, NULL, '2'},
    {"index3", required_argument, NULL, '3'},
    {"index4", required_argument, NULL, '4'},
    {"index5", required_argument, NULL, '5'},
    {"index6", required_argument, NULL, '6'},
    {"index7", required_argument, NULL, '7'},
    {"index8", required_argument, NULL, '8'},
    {"index9", required_argument, NULL, '9'},
    {"index0", required_argument, NULL, '0'},
    {NULL, 0, NULL, 0}};

int print_help(const struct option *longopts, const char *arg0) {
  unsigned int i = 0;
  char *base = strdup(arg0);
  printf("%s", basename(base));
  free(base);
  struct option o = longopts[i];
  char name[DEFAULT_BUFFER_SIZE] = "";
  char value[DEFAULT_BUFFER_SIZE + sizeof("( )") - 1] = "";
  char meta[DEFAULT_BUFFER_SIZE] = "";
  while (o.name != NULL) {
    char *str = meta;

    if (isascii(o.val))
      sprintf(name, "(--%s|-%c)", o.name, (char)o.val);
    else
      sprintf(name, "--%s", o.name);

    sprintf(meta, "%s", o.name);
    do
      *str = (char)toupper(*str);
    while (*str++);

    if (o.has_arg == required_argument)
      sprintf(value, " %s", meta);
    else if (o.has_arg == optional_argument)
      sprintf(value, "( %s)", meta);
    else
      value[0] = '\0';

    printf(" [%s%s]", name, value);

    o = longopts[++i];
  }
  return EXIT_SUCCESS;
}

int feed_keys(char *src) {
#define TMUX_SEND_KEYS "tmux send-keys "
  char command[DEFAULT_BUFFER_SIZE] = TMUX_SEND_KEYS;
  strcpy(command + sizeof(TMUX_SEND_KEYS) - 1, src);
  int ret = system(command);
  if (ret != 0)
    fprintf(stderr, "cannot run: %s", command);
  return ret;
}

int feed_key(char c) {
  char src[2] = {c};
  return feed_keys(src);
}

struct UI {
  char *left;
  char *right;
  char *left_sep;
  char *right_sep;
  char *cursor;
  char *indices[10];
};

int width(char *str) {
  wchar_t wc[DEFAULT_BUFFER_SIZE] = L"";
  size_t wc_len = mbstowcs(wc, str, strlen(str));
  return wcswidth(wc, wc_len);
}

void draw_ui(struct UI ui, RimeContext context) {
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
  printf("\e[2J\r%s%s\e[s%s\r\n"
         "%s%s\e[u",
         left_padding, left, right,
         context.menu.page_no ? ui.left : left_padding, str);
}

int translate(int *c) {
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

int main(int argc, char *argv[]) {
  int c;
  RIME_STRUCT(RimeTraits, traits);
  struct UI ui;
  struct termios newattr, oldattr;
  tcgetattr(STDIN_FILENO, &oldattr);
  newattr = oldattr;
  cfmakeraw(&newattr);
  if (tcsetattr(STDIN_FILENO, TCSANOW, &newattr) == -1)
    err(errno, NULL);

  setlocale(LC_CTYPE, "");
  while ((c = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
    switch (c) {
    case 'V':
      printf("%s " VERSION, argv[0]);
      return 2;
    case 'h':
      print_help(longopts, argv[0]);
      return 1;
    case 's':
      traits.shared_data_dir = optarg;
      break;
    case 'u':
      traits.user_data_dir = optarg;
      break;
    case 'o':
      traits.log_dir = optarg;
      break;
    case 'n':
      traits.distribution_name = optarg;
      break;
    case 'c':
      traits.distribution_code_name = optarg;
      break;
    case 'v':
      traits.distribution_version = optarg;
      break;
    case 'a':
      traits.app_name = optarg;
      break;
    case 'm':
      traits.min_log_level = strtol(optarg, NULL, 0);
      break;
    case 'L':
      ui.left = optarg;
      break;
    case 'R':
      ui.right = optarg;
      break;
    case 'l':
      ui.left_sep = optarg;
      break;
    case 'r':
      ui.right_sep = optarg;
      break;
    case 'I':
      ui.cursor = optarg;
      break;
    case '1':
      ui.indices[0] = optarg;
      break;
    case '2':
      ui.indices[1] = optarg;
      break;
    case '3':
      ui.indices[2] = optarg;
      break;
    case '4':
      ui.indices[3] = optarg;
      break;
    case '5':
      ui.indices[4] = optarg;
      break;
    case '6':
      ui.indices[5] = optarg;
      break;
    case '7':
      ui.indices[6] = optarg;
      break;
    case '8':
      ui.indices[7] = optarg;
      break;
    case '9':
      ui.indices[8] = optarg;
      break;
    case '0':
      ui.indices[9] = optarg;
      break;
    default:
      return -1;
    }
  }
  RimeSetup(&traits);
  RimeInitialize(&traits);
  RimeSessionId session_id = RimeCreateSession();
  if (session_id == 0)
    err(errno, "cannot create session");

  printf("\e[%s q", ui.cursor);
  bool menu_is_empty = true;
  while (true) {
    int mask = 0;
    c = getchar();
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
      feed_key(c);
      continue;
    }
    mask += translate(&c);
    if (!RimeProcessKey(session_id, c, mask)) {
      if (mask == 0)
        feed_key(c);
      continue;
    }
    RIME_STRUCT(RimeContext, context);
    if (!RimeGetContext(session_id, &context)) {
      fputs("cannot get context", stderr);
      continue;
    }
    if (context.menu.num_candidates == 0) {
      menu_is_empty = true;
      RIME_STRUCT(RimeCommit, commit);
      if (RimeCommitComposition(session_id)) {
        if (!RimeGetCommit(session_id, &commit)) {
          fputs("cannot get commit", stderr);
          continue;
        }
        feed_keys(commit.text);
      }
      if (!RimeFreeCommit(&commit)) {
        fputs("cannot free commit", stderr);
        continue;
      }
    } else
      menu_is_empty = false;
    draw_ui(ui, context);
    if (!RimeFreeContext(&context)) {
      fputs("cannot free context", stderr);
      continue;
    }
  }

  if (RimeDestroySession(session_id) == False)
    err(errno, "cannot destroy session");
  if (tcsetattr(STDIN_FILENO, TCSANOW, &newattr) == -1)
    err(errno, NULL);
  return EXIT_SUCCESS;
}
