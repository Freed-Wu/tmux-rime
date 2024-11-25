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
  # https://github.com/NixOS/nixpkgs/issues/314313#issuecomment-2134252094
  shellHook = ''
    LD="$CC"
  '';
}
