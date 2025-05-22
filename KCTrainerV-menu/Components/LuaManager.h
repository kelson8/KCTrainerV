#pragma once
#include <sol/sol.hpp>
#include <string>
#include <optional>

class LuaManager {
public:
#ifdef LUA_TEST

    // Instance of the LuaManager
    //static LuaManager& GetInstance()
    //{
    //    static LuaManager instance;
    //    return instance;
    //}

    static LuaManager& GetInstance();

    LuaManager();
    ~LuaManager();

    // New
    void BindNativesToLua();

#ifdef LUA_NATIVES_TEST
    // Located in GeneratedLuaNativeBindings.cpp.
    void BindAllNativesToLua();
#endif

    // TODO Fix this
    //void BindCustomFunctionsToLua(KCMainScript& scriptContext);
    void BindGameTypesToLua();
    void InitializeLuaEnvironment();

    sol::state& get_state();

    sol::state& GetLuaState(); // Add a way to access the Lua state if needed

    bool load_script(const std::string& filepath);
    sol::optional<sol::function> get_function(const std::string& name);
    // Add other helpful methods as needed (e.g., get_table)

#ifdef LUA_CHAOSMOD_TEST
    void InvokeNativeTest();
#endif // LUA_CHAOSMOD_TEST

private:
    sol::state lua;

    // Singleton pattern
    LuaManager(const LuaManager&) = delete;
    LuaManager& operator=(const LuaManager&) = delete;

#endif //LUA_TEST
};
