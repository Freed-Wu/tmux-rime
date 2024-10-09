# tmux-rime

[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/Freed-Wu/tmux-rime/main.svg)](https://results.pre-commit.ci/latest/github/Freed-Wu/tmux-rime/main)

[![github/downloads](https://shields.io/github/downloads/Freed-Wu/tmux-rime/total)](https://github.com/Freed-Wu/tmux-rime/releases)
[![github/downloads/latest](https://shields.io/github/downloads/Freed-Wu/tmux-rime/latest/total)](https://github.com/Freed-Wu/tmux-rime/releases/latest)
[![github/issues](https://shields.io/github/issues/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime/issues)
[![github/issues-closed](https://shields.io/github/issues-closed/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime/issues?q=is%3Aissue+is%3Aclosed)
[![github/issues-pr](https://shields.io/github/issues-pr/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime/pulls)
[![github/issues-pr-closed](https://shields.io/github/issues-pr-closed/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime/pulls?q=is%3Apr+is%3Aclosed)
[![github/discussions](https://shields.io/github/discussions/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime/discussions)
[![github/milestones](https://shields.io/github/milestones/all/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime/milestones)
[![github/forks](https://shields.io/github/forks/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime/network/members)
[![github/stars](https://shields.io/github/stars/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime/stargazers)
[![github/watchers](https://shields.io/github/watchers/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime/watchers)
[![github/contributors](https://shields.io/github/contributors/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime/graphs/contributors)
[![github/commit-activity](https://shields.io/github/commit-activity/w/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime/graphs/commit-activity)
[![github/last-commit](https://shields.io/github/last-commit/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime/commits)
[![github/release-date](https://shields.io/github/release-date/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime/releases/latest)

[![github/license](https://shields.io/github/license/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime/blob/main/LICENSE)
[![github/languages](https://shields.io/github/languages/count/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime)
[![github/languages/top](https://shields.io/github/languages/top/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime)
[![github/directory-file-count](https://shields.io/github/directory-file-count/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime)
[![github/code-size](https://shields.io/github/languages/code-size/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime)
[![github/repo-size](https://shields.io/github/repo-size/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime)
[![github/v](https://shields.io/github/v/release/Freed-Wu/tmux-rime)](https://github.com/Freed-Wu/tmux-rime)

Rime for tmux.

![screenshot](https://github.com/user-attachments/assets/2887f6a0-1f4e-4d9b-9550-56268cc308b4)

I use [rime.nvim](https://github.com/Freed-Wu/rime.nvim) to input CJKV
characters in neovim. However, for command mode and search mode, it cannot work.
So I create it.

## Dependence

See [shell.nix](shell.nix).

## Build

```sh
xmake
```

## Configure

```tmux
bind -Tprefix C-^ display-popup -Tㄓ -h4 -w122 -xR -yP tmux-rime
```

See

```sh
tmux-rime --help
```

to modify `tmux-rime` to `"tmux-rime [...]"`.

## Install

## AUR

```sh
paru -S tmux-rime
```

## Todo

<!-- markdownlint-disable-next-line MD033 -->
- Support more special keys: <kbd>Up</kbd>, <kbd>Del</kbd>, <kbd>F1</kbd>, ...
- Use C library function to do the work of `tmux send-keys` to save the fork
  cost.
  [Some experiment](https://github.com/Freed-Wu/vimux.nvim) to wrap tmux to a
  library.
- Use a daemon to avoid fork a new process every time

## Related Projects

- [fcitx5-tmux](https://github.com/wengxt/fcitx5-tmux)
