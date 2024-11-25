-- luacheck: ignore 111 113
---@diagnostic disable: undefined-global
add_rules("mode.debug", "mode.release")
add_requires("rime")
add_requires("glib")

includes("@builtin/check")
add_configfiles("config.h.in")
configvar_check_cincludes("RIME_API_DEPRECATED", "rime_api_deprecated.h")

target("tmux-rime")
do
    set_kind("binary")
    add_includedirs("$(buildir)")
    add_files("*.c")
    add_packages("rime")
    add_packages("glib")
end
