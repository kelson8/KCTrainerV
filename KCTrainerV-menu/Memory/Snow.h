#pragma once

// Taken from Chaos Mod in the Memory folder

#include "Constants.hpp"

#ifdef MEMORY_TESTING

#include "Handle.h"
#include "Memory.h"

#include "inc/types.h"
#include "inc/main.h"

typedef unsigned long long DWORD64;
typedef unsigned long DWORD;
typedef unsigned char BYTE;

namespace Memory
{
	inline void SetSnow(bool state)
	{
		/* Thanks to menyoo! */

		static bool init      = false;

		static bool isPre3095 = getGameVersion() < eGameVersion::VER_1_0_3095_0;
		auto snowPattern1 =
		    isPre3095 ? "80 3D ?? ?? ?? ?? 00 74 25 B9 40 00 00 00" : "44 38 ?? ?? ?? ?? 01 74 12 B9 40 00 00 00";
		static auto handle = FindPattern(snowPattern1);
		if (!handle.IsValid())
			return;

		static auto addr1 = handle.Addr();
		static BYTE orig1[13];

		if (!init && addr1)
			memcpy(orig1, reinterpret_cast<void *>(addr1), 13);

		static auto handle2 = FindPattern("44 38 3D ?? ?? ?? ?? 74 1D B9 40 00 00 00");
		if (!handle2.IsValid())
			return;

		static auto addr2 = handle2.Addr();
		static BYTE orig2[14];

		if (!init && addr2)
			memcpy(orig2, reinterpret_cast<void *>(addr2), 14);

		init = true;

		if (state)
		{
			if (addr1)
			{
				BYTE *from                           = reinterpret_cast<BYTE *>(addr1);
				from[0]                              = 0x48; // mov rax, func
				from[1]                              = 0xB8;
				*reinterpret_cast<BYTE **>(&from[2]) = reinterpret_cast<BYTE *>(addr1 + (isPre3095 ? 0x1B : 0x20));
				from[10]                             = 0x50; // push rax
				from[11]                             = 0xC3; // ret
			}

			if (addr2)
			{
				BYTE *from                           = reinterpret_cast<BYTE *>(addr2);
				from[0]                              = 0x48; // mov rax, func
				from[1]                              = 0xB8;
				*reinterpret_cast<BYTE **>(&from[2]) = reinterpret_cast<BYTE *>(addr2 + 0x1C);
				from[10]                             = 0x50; // push rax
				from[11]                             = 0xC3; // ret
			}
		}
		else
		{
			if (addr1)
				memcpy(reinterpret_cast<void *>(addr1), orig1, 13);

			if (addr2)
				memcpy(reinterpret_cast<void *>(addr2), orig2, 14);
		}
	}
}

#endif // MEMORY_TESTING