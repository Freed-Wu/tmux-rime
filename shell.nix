{ pkgs ? import <nixpkgs> { } }:

with pkgs;
mkShell {
  name = "tmux-rime";
  buildInputs = [
    pkg-config
    stdenv.cc
    librime
    xmake
    tmux
  ];
}
