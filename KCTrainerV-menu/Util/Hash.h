#pragma once

// https://github.com/gta-chaos-mod/ChaosModV/blob/master/ChaosMod/Util/Hash.h

#include <inc/natives.h>

using Hash   = unsigned long;
using size_t = unsigned long long;

// Well, I removed constexpr and this seems to have fixed this for now.
// Making this inline seems to have solved this, now I can use it in multiple files.

/// <summary>
/// Convert a string to a hash like this: "weapon_snowball"_hash
/// </summary>
/// <param name="str"></param>
/// <param name="n"></param>
/// <returns></returns>
inline Hash operator""_hash(const char *str, size_t n)
{
	return MISC::GET_HASH_KEY(str);
}

// TODO Are these needed?
//static_assert("prop_golf_ball"_hash == 0xAF0E3F9F);
//static_assert("PROP_GOLF_BALL"_hash == 0xAF0E3F9F);