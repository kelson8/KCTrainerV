#include "LuaManager.h"

#ifdef LUA_TEST
#include <sol/sol.hpp>

#include <inc/natives.h>
#include "Natives/natives2.h"

#include "Util/Logger.hpp"

// TODO Test this later.
// I have a lot of Chaos Mod functions adapted at the bottom of this file that should work.

LuaManager g_lua_manager; // Define the global instance

LuaManager::LuaManager() {
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug, sol::lib::bit32);
    // Bind C++ functions or natives here if they need to be available immediately
    lua.set_function("TRIGGER_MUSIC_EVENT", AUDIO::TRIGGER_MUSIC_EVENT);
    // ... other bindings ...
}

LuaManager::~LuaManager() {
    // Clean up Lua state if needed (though Sol2 handles it well)
}

sol::state& LuaManager::get_state() {
    return lua;
}

bool LuaManager::load_script(const std::string& filepath) {
    sol::load_result lr = lua.load_file(filepath);
    if (!lr.valid()) {
        sol::error err = lr;
        // LOG(ERROR, std::format("Error loading Lua script {}: {}", filepath, err.what()));
        return false;
    }
    sol::protected_function pf = lr;
    sol::protected_function_result pfr = pf();
    if (!pfr.valid()) {
        sol::error err = pfr;
        // LOG(ERROR, std::format("Error executing Lua script {}: {}", filepath, err.what()));
        return false;
    }
    return true;
}

sol::optional<sol::function> LuaManager::get_function(const std::string& name) {
    sol::function func = lua[name];
    if (func.valid()) {
        return func;
    }
    return sol::nullopt;
}

// Below copied from LuaScripts.cpp in Chaos Mod

// MinGW doesn't have SEH :(
#ifdef _MSC_VER
#define MAGIC_CATCH_BEGIN \
	__try                 \
	{
#define MAGIC_CATCH_END(x)               \
	}                                    \
	__except (EXCEPTION_EXECUTE_HANDLER) \
	{                                    \
		x;                               \
	}
#else
#define MAGIC_CATCH_BEGIN {
#define MAGIC_CATCH_END(x) }
#endif

//void LuaPrint(const std::string& text)
//{
//	//COLOR_PREFIX_LOG("(Lua)", text);
//	COLOR_PREFIX_LOG("(Lua)", text);
//}
//
//void LuaPrint(const std::string& name, const std::string& text)
//{
//	COLOR_PREFIX_LOG("(" << name << ")", text);
//
//#ifdef WITH_DEBUG_PANEL_SUPPORT
//	if (ComponentExists<DebugSocket>())
//		GetComponent<DebugSocket>()->ScriptLog(name, text);
//#endif
//}

static char* _TryParseString(void* str)
{
	auto string = reinterpret_cast<char*>(str);

	MAGIC_CATCH_BEGIN
		// Access string to try to trigger an access violation
		for (char* c = string; *c; c++)
		{
		}
	MAGIC_CATCH_END(return nullptr)

		return string;
}

static bool _TryParseVector3(void** vector, float& x, float& y, float& z)
{
	MAGIC_CATCH_BEGIN
		x = *reinterpret_cast<float*>(vector);
	y = *reinterpret_cast<float*>(vector + 1);
	z = *reinterpret_cast<float*>(vector + 2);
	MAGIC_CATCH_END(return false)

		return true;
}

static bool _CallNative(void*** result)
{
	MAGIC_CATCH_BEGIN
		* result = reinterpret_cast<void**>(nativeCall());
	MAGIC_CATCH_END(return false)

		return true;
}

template <typename T, typename... A> static T Generate(const A &&...args)
{
	return T(args...);
}

struct LuaVector3
{
	alignas(8) float X = 0.f;
	alignas(8) float Y = 0.f;
	alignas(8) float Z = 0.f;

	LuaVector3() = default;

	LuaVector3(float x, float y, float z) : X(x), Y(y), Z(z)
	{
	}
};

class LuaHolder
{
public:
	void* m_Data = nullptr;
	sol::object m_Obj;

	LuaHolder() = default;

	LuaHolder(const sol::object& obj) : m_Obj(obj)
	{
	}

	~LuaHolder()
	{
		if (m_Obj.valid())
			m_Obj.abandon();
	}

	template <typename T> __forceinline T As()
	{
		if constexpr (std::is_same<T, char*>())
		{
			return _TryParseString(m_Data);
		}
		else if constexpr (std::is_same<T, LuaVector3>())
		{
			float x, y, z;

			return _TryParseVector3(&m_Data, x, y, z) ? LuaVector3(x, y, z) : LuaVector3();
		}

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif
		return *reinterpret_cast<T*>(&m_Data);
	}

	__forceinline bool IsValid() const
	{
		return m_Data || m_Obj.valid();
	}
};



enum class LuaNativeReturnType
{
	None,
	Bool,
	Int,
	UInt,
	Float,
	String,
	Vector3
};

// TODO Adapt to possibly make all natives work
// TODO Attempt to replicate this part in KCTrainerV, this might allow me to run all natives in lua
// TODO Test this, I got this to build like this.
static sol::object LuaInvoke(const sol::environment& env, uint64_t nativeHash, LuaNativeReturnType returnType,
	const sol::variadic_args& args)
{
	if (nativeHash == 0x213AEB2B90CBA7AC || nativeHash == 0x5A5F40FE637EB584 || nativeHash == 0x933D6A9EEC1BACD0
		|| nativeHash == 0xE80492A9AC099A93 || nativeHash == 0x8EF07E15701D61ED)
	{
		return sol::make_object(env.lua_state(), sol::lua_nil);
	}

	nativeInit(nativeHash);

	for (const sol::stack_proxy& arg : args)
	{
		if (arg.is<bool>())
		{
			nativePush(arg.get<bool>());
		}
		else if (arg.is<int>())
		{
			nativePush(arg.get<int>());
		}
		else if (arg.is<float>())
		{
			nativePush(arg.get<float>());
		}
		else if (arg.is<const char*>())
		{
			nativePush(arg.get<const char*>());
		}
		else if (arg.is<LuaHolder>())
		{
			LuaHolder& holder = arg.get<LuaHolder>();

			if (holder.m_Obj.valid())
			{
				DWORD64 data = holder.m_Obj.as<DWORD64>();

				nativePush(&data);
			}
			else
			{
				nativePush(&holder.m_Data);
			}
		}
	}

	void** returned;
	if (!_CallNative(&returned))
	{
		LOG(ERROR, "Error invoking native 0x", nativeHash);
		//LuaPrint(env.get<sol::table>("EnvInfo")["ScriptName"],
		//	(std::ostringstream() << "Error while invoking native 0x" << std::uppercase << std::hex << nativeHash)
		//	.str());
	}
	else if (returned)
	{
		switch (returnType)
		{
		case LuaNativeReturnType::Bool:
			return sol::make_object(env.lua_state(), *reinterpret_cast<bool*>(returned));
		case LuaNativeReturnType::Int:
			return sol::make_object(env.lua_state(), *reinterpret_cast<int*>(returned));
		case LuaNativeReturnType::UInt:
			return sol::make_object(env.lua_state(), *reinterpret_cast<int*>(returned));
		case LuaNativeReturnType::Float:
			return sol::make_object(env.lua_state(), *reinterpret_cast<float*>(returned));
		case LuaNativeReturnType::String:
			return sol::make_object(env.lua_state(), _TryParseString(*returned));
		case LuaNativeReturnType::Vector3:
		{
			LuaVector3 vector3;
			if (_TryParseVector3(returned, vector3.X, vector3.Y, vector3.Z))
				return sol::make_object(env.lua_state(), vector3);
		}
		break;
		default:
			break;
		}
	}

	return sol::make_object(env.lua_state(), sol::lua_nil);
}

//

#endif //LUA_TEST