#pragma once
#include "Constants.hpp"

#include <sol/sol.hpp>
#include <string>
#include <optional>

class LuaManager {
public:
#ifdef LUA_TEST

    // Instance of the LuaManager
    static LuaManager& GetInstance()
    {
        static LuaManager instance;
        return instance;
    }

    LuaManager();
    ~LuaManager();

    // New
    void BindNativesToLua();
    void InitializeLuaEnvironment();

    sol::state& get_state();
    bool load_script(const std::string& filepath);
    sol::optional<sol::function> get_function(const std::string& name);
    // Add other helpful methods as needed (e.g., get_table)

private:
    sol::state lua;

    //LuaManager() {} // Private constructor to prevent external instantiation
    //LuaManager(const LuaManager&) = delete;
    //LuaManager& operator=(const LuaManager&) = delete;
#endif //LUA_TEST
};

#ifdef LUA_TEST
extern LuaManager m_GlobalState; // Global instance of the manager
#endif //LUA_TEST