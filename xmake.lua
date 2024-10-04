-- luacheck: ignore 111 113
---@diagnostic disable: undefined-global
add_rules("mode.debug", "mode.release")
add_requires("rime")

local opts = {}
local quoted_opts = {}
local opt

opt = "key-table"
table.insert(opts, opt)
option(opt)
do
    set_category("tmux/bind-key")
    -- set_default("$(shell tmux show-options -gv key-table)")
    set_default("prefix")
    set_values { "prefix", "root", "copy-mode", "copy-mode-vi" }
    set_description("which table to bind key")
end

opt = "key"
table.insert(opts, opt)
option(opt)
do
    set_category("tmux/bind-key")
    set_default("C-^")
    set_description("hot key")
end

opt = "popup-border-lines"
table.insert(opts, opt)
option(opt)
do
    set_category("tmux/display-popup")
    set_default("$(shell tmux show-options -gv popup-border-lines)")
    set_values { "single", "rounded", "double", "heavy", "simple", "padded", "none" }
    set_description("popup border lines")
end

opt = "title"
table.insert(opts, opt)
option(opt)
do
    set_category("tmux/display-popup")
    set_default("ㄓ")
    set_description("popup window title")
end

opt = "x"
table.insert(opts, opt)
option(opt)
do
    set_category("tmux/display-popup")
    set_default("R")
    set_description("popup window x-coordinate")
end

opt = "y"
table.insert(opts, opt)
option(opt)
do
    set_category("tmux/display-popup")
    set_default("P")
    set_description("popup window y-coordinate")
end

opt = "height"
table.insert(opts, opt)
option(opt)
do
    set_category("tmux/display-popup")
    set_default("2")
    set_description("popup window height")
    add_deps("popup-border-lines")
    after_check(function(option)
        if option:dep("popup-border-lines"):value() ~= "none" then
            option:set_value(tostring(tonumber(option:value()) + 2))
        end
    end)
end

opt = "width"
table.insert(opts, opt)
option(opt)
do
    set_category("tmux/display-popup")
    set_default("120")
    set_description("popup window width")
    add_deps("popup-border-lines")
    after_check(function(option)
        if option:dep("popup-border-lines"):value() ~= "none" then
            option:set_value(tostring(tonumber(option:value()) + 2))
        end
    end)
end

opt = "shared_data_dir"
table.insert(quoted_opts, opt)
option(opt)
do
    set_category("rime/traits")
    set_description("shared data dir")
    for _, v in pairs {
        "/usr/share/rime-data",
        "/usr/local/share/rime-data",
        "/run/current-system/sw/share/rime-data",
        "/sdcard/rime-data"
    } do
        ---@diagnostic disable: undefined-field
        -- luacheck: ignore 143
        if os.isdir(v) then
            set_default(v)
            break
        end
    end
end

opt = "user_data_dir"
table.insert(quoted_opts, opt)
option(opt)
do
    set_category("rime/traits")
    set_description("user data dir")
    for _, v in pairs { "~/.config/ibus/rime", "~/.local/share/fcitx5/rime", "~/.config/fcitx/rime", "/sdcard/rime" } do
        if os.isdir(v) then
            set_default(v)
            break
        end
    end
end

opt = "log_dir"
table.insert(quoted_opts, opt)
option(opt)
do
    set_category("rime/traits")
    set_description("log dir")
    set_default((os.getenv("XDG_DATA_DIR") or "~/.local/share") .. "/tmux/rime")
    after_check(
        function(option)
            os.mkdir(option:value())
        end
    )
end

opt = "left"
table.insert(quoted_opts, opt)
option(opt)
do
    set_category("rime/UI")
    set_description("symbol for previous menu")
    set_default("'<|'")
end

opt = "right"
table.insert(quoted_opts, opt)
option(opt)
do
    set_category("rime/UI")
    set_description("symbol for next menu")
    set_default("'|>'")
end

opt = "left_sep"
table.insert(quoted_opts, opt)
option(opt)
do
    set_category("rime/UI")
    set_description("left separator")
    set_default("[")
end

opt = "right_sep"
table.insert(quoted_opts, opt)
option(opt)
do
    set_category("rime/UI")
    set_description("right separator")
    set_default("]")
end

opt = "cursor"
table.insert(quoted_opts, opt)
option(opt)
do
    set_category("rime/UI")
    set_description("cursor")
    set_default("blinking bar")
    set_values {
        "steady block",
        "steady block 2",
        "blinking block",
        "steady underline",
        "blinking underline",
        "steady bar",
        "blinking bar"
    }
    after_check(
        function(option)
            for i, value in ipairs(option:get("values")) do
                if option:value() == value then
                    option:set_value(i - 1)
                end
            end
        end
    )
end

for i, v in ipairs { "① ", "②", "③", "④", "⑤", "⑥", "⑦", "⑧", "⑨", "⓪" } do
    opt = "index " .. tostring(i)
    table.insert(quoted_opts, opt)
    option(opt)
    do
        set_category("rime/UI/indices")
        set_description("symbol for " .. opt)
        set_default(v)
    end
end

opt = "distribution_name"
table.insert(quoted_opts, opt)
option(opt)
do
    set_category("rime/traits")
    set_description("distribution name")
    set_default("Rime")
end

opt = "distribution_code_name"
table.insert(quoted_opts, opt)
option(opt)
do
    set_category("rime/traits")
    set_description("distribution code name")
    set_default("tmux-rime")
end

opt = "distribution_version"
table.insert(quoted_opts, opt)
option(opt)
do
    set_category("rime/traits")
    set_description("distribution version")
    set_default("0.0.1")
end

opt = "app_name"
table.insert(quoted_opts, opt)
option(opt)
do
    set_category("rime/traits")
    set_description("app name")
    set_default("rime.tmux-rime")
end

opt = "min_log_level"
table.insert(quoted_opts, opt)
option(opt)
do
    set_category("rime/traits")
    set_description("min log level")
    set_default("FATAL")
    set_values { "INFO", "WARNING", "ERROR", "FATAL" }
    after_check(
        function(option)
            for i, value in ipairs(option:get("values")) do
                if option:value() == value then
                    option:set_value(i - 1)
                end
            end
        end
    )
end

target("tmux-rime")
do
    set_kind("binary")
    add_files("*.c")
    add_packages("rime")
    set_configdir("$(projectdir)")
    add_configfiles("rime.tmux.in")
    set_configvar("curdir", "$(curdir)")
    for _, o in ipairs(opts) do
        set_configvar(o, get_config(o))
    end
    for _, o in ipairs(quoted_opts) do
        set_configvar(o, get_config(o), { quote = true })
    end
    on_config(
        function(target)
            target:set("configvar", "targetfile", target:targetfile())
        end
    )
end
