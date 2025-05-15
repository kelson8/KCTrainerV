#include "PedScripts.h"

#include "Util/EntityIterator.h"

/// <summary>
/// Set all peds as cops.
/// Makes all peds in the area have a cop icon on the radar.
/// Taken from Chaos Mod
/// </summary>
void PedScripts::SetAllPedsAsCops()
{
	for (Ped ped : GetAllPeds())
	{
		Ped pedType = PED::GET_PED_TYPE(ped);

		if (!PED::IS_PED_A_PLAYER(ped) && pedType != 0 && pedType != 1 && pedType != 2)
			PED::SET_PED_AS_COP(ped, true);
	}
}