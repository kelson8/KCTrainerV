#pragma once
#include "Constants.hpp"

#include <sol/sol.hpp>
#include <string>
#include <optional>

class LuaManager {
public:
#ifdef LUA_TEST
    LuaManager();
    ~LuaManager();

    sol::state& get_state();
    bool load_script(const std::string& filepath);
    sol::optional<sol::function> get_function(const std::string& name);
    // Add other helpful methods as needed (e.g., get_table)

private:
    sol::state lua;
#endif //LUA_TEST
};

#ifdef LUA_TEST
extern LuaManager g_lua_manager; // Global instance of the manager
#endif //LUA_TEST