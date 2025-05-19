#include "pch.h"

// From ChaosMod: https://github.com/gta-chaos-mod/ChaosModV/tree/master/ChaosMod/Memory

// Original files
#include "Util/Logger.hpp"
//


// Chaos mod files
#include "Memory.h"

//#include "Effects/EffectThreads.h"

#include "Memory/Hooks/Hook.h"

// Libraries
#include <minhook/include/MinHook.h>
#include <Patterns/Patterns.h>

// Hmm, these give already defined symbols.
// Seems to build if I don't have it in the project folder within visual studio?
// I wonder if it'll actually work.
#include <shv/exports.h>


//#include "Util/Script.h"
#include "Util/Text.h"
#include "Util/File.h"
//

#ifdef MEMORY_TESTING

static DWORD64 ms_BaseAddr;
static DWORD64 ms_EndAddr;

static std::set<std::string> ms_BlacklistedHookNames;

// TODO Test this

namespace Memory
{
	void Init()
	{
		MODULEINFO moduleInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &moduleInfo, sizeof(moduleInfo));

		ms_BaseAddr = reinterpret_cast<DWORD64>(moduleInfo.lpBaseOfDll);
		ms_EndAddr  = ms_BaseAddr + moduleInfo.SizeOfImage;

		//MH_Initialize();

		// TODO Test this
		if (MH_Initialize() != MH_OK) {
			// Output an error message (if possible at this stage)
			if (AllocConsole()) {
				std::cerr << "MinHook initialization failed!" << std::endl;
			}
			else {
				OutputDebugStringA("MinHook initialization failed! (No console)");
			}
		}
		// ... your script registration code ...


		// TODO Figure out how to use these feature flags, could be useful.
		if (DoesFeatureFlagExist("skipintro"))
		{
			// Splash screen
			Handle handle = FindPattern("E8 ? ? ? ? 8B CF 40 88 2D");
			if (!handle.IsValid())
			{
				LOG(WARN, "SkipIntro: Failed to patch splash screen!");
			}
			else
			{
				Write<BYTE>(handle.Into().At(0x21).Into().Get<BYTE>(), 0x0, 36);

				LOG(INFO, "SkipIntro: Patched splash screen");
			}

			// Legal screen
			handle = FindPattern("E8 ? ? ? ? EB 0D B1 01");
			if (!handle.IsValid())
			{
				LOG(WARN, "SkipIntro: Failed to patch legal screen!");
			}
			else
			{
				handle = handle.Into();

				Write<BYTE>(handle.Get<BYTE>(), 0xC3);
				Write<BYTE>(handle.At(0x9).Into().At(0x3).Get<BYTE>(), 0x2);

				LOG(INFO, "SkipIntro: Patched legal screen");
			}
		}

		if (DoesFeatureFlagExist("skipdlcs"))
		{
			Handle handle = FindPattern("84 C0 74 2C 48 8D 15 ? ? ? ? 48 8D 0D ? ? ? ? 45 33 C9 41 B0 01");
			if (!handle.IsValid())
			{
				LOG(WARN, "SkipDLCs: Failed to patch DLC loading!");
			}
			else
			{
				Write<BYTE>(handle.At(24).Get<BYTE>(), 0x90, 24);

				LOG(INFO, "SkipDLCs: Patched DLC loading");
			}
		}

		// TODO Test this, I need to set it up for my menu.
		if (DoesFeatureFlagExist("blacklistedhooks"))
		{
			//std::ifstream file("chaosmod\\.blacklistedhooks");
			std::string fileName = std::string(Constants::ScriptFolder) + "\\.blacklistedhooks";

			//std::ifstream file(Constants::ScriptFolder + "\\.blacklistedhooks");
			std::ifstream file(fileName);

			if (!file.fail())
			{
				std::string line;
				line.resize(64);
				while (file.getline(line.data(), 64))
					ms_BlacklistedHookNames.insert(StringTrim(line.substr(0, line.find("\n"))));
			}
		}

		LOG(INFO, "Running hooks");
		std::thread(
		    []()
		    {
			    for (auto registeredHook = g_pRegisteredHooks; registeredHook;
			         registeredHook      = registeredHook->GetNext())
			    {
				    if (registeredHook->IsLateHook())
					    continue;

				    const auto &hookName = registeredHook->GetName();

				    if (ms_BlacklistedHookNames.contains(hookName))
				    {
					    //LOG(INFO, hookName << " hook has been blacklisted from running!");
					    LOG(INFO, "{} hook has been blacklisted from running!", hookName);
					    continue;
				    }

				    //LOG(INFO, "Running " << hookName << " hook");
				    LOG(INFO, "Running {} hook", hookName);

				    if (!registeredHook->RunHook())
					    //LOG(WARN, hookName << " hook failed!");
					    LOG(WARN, "{} hook failed!", hookName);
			    }

			    MH_EnableHook(MH_ALL_HOOKS);
		    })
		    .detach();
	}

	void Uninit()
	{
		LOG(INFO, "Running hook cleanups");
		for (auto registeredHook = g_pRegisteredHooks; registeredHook; registeredHook = registeredHook->GetNext())
		{
			const auto &hookName = registeredHook->GetName();

			if (ms_BlacklistedHookNames.contains(hookName))
				continue;

			//LOG(INFO, "Running " << hookName << " hook cleanup");
			LOG(INFO, "Running {} hook cleanup", hookName);

			registeredHook->RunCleanup();
		}

		MH_DisableHook(MH_ALL_HOOKS);

		MH_Uninitialize();
	}

	void RunLateHooks()
	{
		LOG(INFO, "Running late hooks");

		std::thread(
		    []()
		    {
			    for (auto registeredHook = g_pRegisteredHooks; registeredHook;
			         registeredHook      = registeredHook->GetNext())
			    {
				    if (!registeredHook->IsLateHook())
					    continue;

				    const auto &hookName = registeredHook->GetName();

				    if (ms_BlacklistedHookNames.contains(hookName))
				    {
					    //LOG(INFO, hookName << " hook has been blacklisted from running!");
					    LOG(INFO,  "{} hook has been blacklisted from running!", hookName);
					    continue;
				    }

				    //LOG(INFO, "Running " << hookName << " hook");
				    LOG(INFO, "Running {} hook", hookName);

				    if (!registeredHook->RunHook())
					    LOG(WARN, "{} hook failed!", hookName);
			    }

			    MH_EnableHook(MH_ALL_HOOKS);
		    })
		    .detach();
	}

	Handle FindPattern(const std::string &pattern, const PatternScanRange &&scanRange)
	{
		DEBUG_LOG("Searching for pattern \"{}\"{}",
			pattern,
			(scanRange.StartAddr == 0 && scanRange.EndAddr == 0
				? ""
				: (std::stringstream()
					<< " within address range 0x" << std::uppercase << std::hex << scanRange.StartAddr
					<< std::setfill(' ') << " to 0x" << std::uppercase << std::hex << scanRange.EndAddr)
				.str()));

		if ((scanRange.StartAddr != 0 || scanRange.EndAddr != 0) && scanRange.StartAddr >= scanRange.EndAddr)
		{
			LOG(WARN, "startAddr is equal / bigger than endAddr???");
			return Handle();
		}

		auto scanPattern = [&]() -> Handle
		{
			auto copy = pattern;
			for (size_t pos = copy.find("??"); pos != std::string::npos; pos = copy.find("??", pos + 1))
				copy.replace(pos, 2, "?");

			auto thePattern = scanRange.StartAddr == 0 && scanRange.EndAddr == 0
			                    ? hook::pattern(copy)
			                    : hook::pattern(scanRange.StartAddr, scanRange.EndAddr, copy);
			if (!thePattern.size())
				return {};

			auto resultAddr = reinterpret_cast<uintptr_t>(thePattern.get_first());

			DEBUG_LOG("Found pattern \"{}\" at address 0x{:X}", pattern, resultAddr);
			
			return resultAddr;
		};

		// TODO Implement this, chaos mode effects.
		//if (EffectThreads::IsThreadAnEffectThread())
		//{
		//	Handle handle;

		//	auto future = std::async(std::launch::async, [&]() { handle = scanPattern(); });

		//	using namespace std::chrono_literals;
		//	while (future.wait_for(0ms) != std::future_status::ready)
		//		WAIT(0);

		//	return handle;
		//}

		return scanPattern();
	}

	const char *GetTypeName(__int64 vftAddr)
	{
		if (vftAddr)
		{
			auto vftable = *reinterpret_cast<__int64 *>(vftAddr);
			if (vftable)
			{
				auto rtti = *reinterpret_cast<__int64 *>(vftable - 8);
				if (rtti)
				{
					auto rva = *reinterpret_cast<DWORD *>(rtti + 12);
					if (rva)
					{
						auto typeDesc = ms_BaseAddr + rva;
						if (typeDesc)
							return reinterpret_cast<const char *>(typeDesc + 16);
					}
				}
			}
		}

		return "UNK";
	}

	DWORD64 *GetGlobalPtr(int globalId)
	{
		static auto globalPtr = []() -> DWORD64 **
		{
			auto handle = FindPattern("4C 8D 05 ? ? ? ? 4D 8B 08 4D 85 C9 74 11");
			if (!handle.IsValid())
				return nullptr;

			return handle.At(2).Into().Get<DWORD64 *>();
		}();

		static auto fallbackToSHV = []() -> bool
		{
			bool fallbackToSHV = !globalPtr;

			if (fallbackToSHV)
			{
				LOG(WARN, "Warning: _globalPtr not found, falling back to SHV's getGlobalPtr");
			}
			// HACK: Check for the presence some arbitrary module specific to FiveM
			// Also check if player is in a mp session if so to check for FiveM sp
			//else if (GetModuleHandle(L"gta-net-five.dll"))
			else if (GetModuleHandle("gta-net-five.dll"))
			{
				bool modeDetermined = false;

				auto handle         = FindPattern("48 8B 0D ? ? ? ? E8 ? ? ? ? 84 C0 74 09 48 8D 15");
				if (handle.IsValid())
				{
					auto _networkObj = handle.At(2).Into().Get<DWORD64>();
					handle           = FindPattern("83 B9 ? ? 00 00 05 0F 85 ? ? ? ? E9");
					if (handle.IsValid())
					{
						fallbackToSHV  = *reinterpret_cast<DWORD *>(*_networkObj + handle.At(2).Value<WORD>()) == 5;
						modeDetermined = true;
					}
				}

				if (!modeDetermined)
				{
					LOG(WARN, "Warning: FiveM detected but could not determine mode, switching to fallback for GetGlobalPtr");
					fallbackToSHV = true;
				}
			}

			if (fallbackToSHV)
				LOG(WARN, "Warning: FiveM (non-sp) detected, features such as Failsafe will not work!");

			return fallbackToSHV;
		}();

		return fallbackToSHV ? getGlobalPtr(globalId) : (&globalPtr[globalId >> 18 & 0x3F][globalId & 0x3FFFF]);
	}

	std::string GetGameBuild()
	{
		static auto gameBuild = []() -> std::string
		{
			auto handle = Memory::FindPattern("80 3D ? ? ? ? 00 0F 57 C0 48");
			if (!handle.IsValid())
				return {};

			std::string buildStr = handle.At(1).Into().At(1).Get<char>();
			if (buildStr.empty())
				return {};

			auto splitIndex = buildStr.find("-dev");
			if (splitIndex == buildStr.npos)
				return {};

			return buildStr.substr(0, splitIndex);
		}();

		return gameBuild;
	}
}

#endif //MEMORY_TESTING