-- luacheck: ignore 111 113
---@diagnostic disable: undefined-global
add_rules("mode.debug", "mode.release")
add_requires("rime")
add_requires("glib")

target("tmux-rime")
do
    set_kind("binary")
    add_files("src/main.c")
    add_deps("lib")
end

target("lib")
do
    set_kind("static")
    set_basename("tmux-rime")
    add_files("src/tmux-rime.c")
    add_installfiles("src/*.h", { prefixdir = "include" })
    add_packages("rime")
    add_packages("glib")
end
