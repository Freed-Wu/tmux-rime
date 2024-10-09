-- luacheck: ignore 111 113
---@diagnostic disable: undefined-global
add_rules("mode.debug", "mode.release")
add_requires("rime")
add_requires("glib")

target("tmux-rime")
do
    set_kind("binary")
    add_files("*.c")
    add_packages("rime")
    add_packages("glib")
    after_build(
        function(target)
            ---@diagnostic disable: undefined-field
            -- luacheck: ignore 143
            os.rm("tmux-rime")
            os.ln(target:targetfile(), "tmux-rime")
        end
    )
end
