#pragma once
#include "Constants.hpp"

#include <vector>
#include <string>
#include <string_view> // For std::string_view
#include <map>


#include <inc/natives.h>
#include <inc/types.h>

namespace Teleports
{
	// TODO Set this up, store IPLs in here instead of directly in the new teleport formats.
	namespace Ipls
	{
		extern const std::vector<std::string> vCasinoNightclubIpls;
	} // namespace Ipls
} // namespace Teleports