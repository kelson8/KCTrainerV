#pragma once
#include "Constants.hpp"

#include "Util/Enums.h"


// TODO Refactor this file a bit, possibly make a Config class under the IDGun namespace for the variables like before.

namespace MiscScripts
{
	namespace IDGun
	{
		void Tick();

		// ID Gun tests from FiveM
		void IdGun();
		Entity GetEntityIsAimingAt(Ped ped);
		void ResetIdGunPositions();

		//bool isIdGunEnabled = false;
		extern bool isIdGunEnabled;

		//float entityIdMenuPosX = 0.190f;
		extern float entityIdMenuPosX;
		//float entityIdMenuPosY = 0.75f;
		extern float entityIdMenuPosY;

		//float entityCoordsMenuPosX = 0.190f;
		extern float entityCoordsMenuPosX;
		//float entityCoordsMenuPosY = 0.80f;
		extern float entityCoordsMenuPosY;

		//float entityHeadingMenuPosX = 0.190f;
		extern float entityHeadingMenuPosX;
		//float entityHeadingMenuPosY = 0.85f;
		extern float entityHeadingMenuPosY;

		//float entityModelMenuPosX = 0.190f;
		extern float entityModelMenuPosX;
		//float entityModelMenuPosY = 0.9f;
		extern float entityModelMenuPosY;

		//
	} // namespace IDGun
}