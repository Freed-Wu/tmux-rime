{ pkgs ? import <nixpkgs> { } }:

with pkgs;
mkShell {
  name = "tmux-rime";
  buildInputs = [
    pkg-config
    stdenv.cc
    librime
    glib.dev
    xmake
    tmux
  ];
}
