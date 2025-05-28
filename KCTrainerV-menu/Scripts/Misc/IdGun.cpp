#include "pch.h"

#include "IdGun.h"

#include "Constants.hpp"

//#include "MiscScripts.h"
#include "Scripts/PlayerScripts.h"
#include "Scripts/TextScripts.h"
#include "Scripts/VehicleScripts.h"

#include <format>

#include "inc/types.h"

#include "Util/UI.hpp"
#include "Util/Util.hpp"
#include "Script.hpp"

//#include "../Teleports/TeleportLocations.h"

// Test for lua
#ifdef LUA_TEST
#include "Components/LuaManager.h"
#endif //LUA_TEST

// Menyoo
#include "GTAped.h"
#include "GTAentity.h"
#include "GTAvehicle.h"
#include "Tasks.h"

// Begin adapted from pun_idgun FiveM

namespace MiscScripts
{
	namespace IDGun
	{
		bool isIdGunEnabled = false;

		// New values
		float entityIdMenuPosX = 0.162f;
		float entityIdMenuPosY = 0.858f;

		float entityCoordsMenuPosX = 0.162f;
		float entityCoordsMenuPosY = 0.889f;

		float entityHeadingMenuPosX = 0.162f;
		float entityHeadingMenuPosY = 0.919f;

		float entityModelMenuPosX = 0.162f;
		float entityModelMenuPosY = 0.951f;
		//

		/// <summary>
		/// Main tick for the IDGun
		/// </summary>
		void Tick()
		{
			//-----
			// This mostly works, draw entity id, entity x,y,z, entity heading, and entity model hash to the screen.
			// Also draw vehicle name to the screen.
			//-----
			if (isIdGunEnabled)
			{
				IdGun();
			}
		}

		Entity GetEntityIsAimingAt(Ped ped)
		{
			// Blank value for function
			Entity entity;
			Entity isAimingAt = GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(ped, &entity);
			//return isAimingAt;
			return entity;
		}

		/// <summary>
		/// Reset values back to defaults.
		/// </summary>
		void ResetIdGunPositions()
		{
			entityIdMenuPosX = 0.162f;
			entityIdMenuPosY = 0.858f;

			entityCoordsMenuPosX = 0.162f;
			entityCoordsMenuPosY = 0.889f;

			entityHeadingMenuPosX = 0.162f;
			entityHeadingMenuPosY = 0.919f;

			entityModelMenuPosX = 0.162f;
			entityModelMenuPosY = 0.951f;

		}

		/// <summary>
		/// This seems to mostly work now.
		/// This will get the ped names and object names as hash keys instead of the numerical hash ids now.
		/// This works for getting vehicle names now.
		/// Shows these values:
		/// Entity ID
		/// Entity X, Y, Z
		/// Entity Heading
		/// Entity Hash (Model name hash)
		/// Also can show Ped name, and Object name.
		/// Adapted from here: https://forum.cfx.re/t/id-gun-find-out-object-coords-headings-and-hashes/984257
		/// </summary>
		void IdGun()
		{
			auto& textScripts = TextScripts::GetInstance();

			auto& playerScripts = PlayerScripts::GetInstance();

			auto& vehicleScripts = VehicleScripts::GetInstance();

			Util util = Util();

			//Ped playerPed = playerScripts.GetPlayerPed();
			// I think this fixed it, I think I needed the playerID
			Player playerPed = PLAYER_ID();


			// Check if the player is aiming
			if (IS_PLAYER_FREE_AIMING(playerPed))
			{
				// Aimed at entity
				Entity entity = GetEntityIsAimingAt(playerPed);

				// Entity coords and heading
				Vector3 entityCoords = GET_ENTITY_COORDS(entity, false);
				float entityHeading = GET_ENTITY_HEADING(entity);

				// Entity hash
				Hash entityHash = GET_ENTITY_MODEL(entity);

				//------
				// Entity ID
				//------
				std::string entityIdString = std::format("Entity ID: {}", std::to_string(entity));
				textScripts.SetTextEntry(entityIdString.c_str());
				textScripts.TextPosition(entityIdMenuPosX, entityIdMenuPosY);

				//------
				// Entity coords
				//------
				std::string entityCoordsString = std::format("Entity Coords: X: {:.2f} Y: {:.2f} Z: {:.2f}",
					entityCoords.x, entityCoords.y, entityCoords.z);
				textScripts.SetTextEntry(entityCoordsString.c_str());

				textScripts.TextPosition(entityCoordsMenuPosX, entityCoordsMenuPosY);

				//------
				// Entity heading
				//------
				std::string entityHeadingString = std::format("Entity Heading: {}", std::to_string(entityHeading));
				textScripts.SetTextEntry(entityHeadingString.c_str());

				textScripts.TextPosition(entityHeadingMenuPosX, entityHeadingMenuPosY);

				//------
				// Display vehicle name or entity model hash
				//------
				// If the entity is a vehicle, show the name of the vehicle instead of the entity model hash.
				// I pretty much got this working

				// I adapted this from the pun_idgun FiveM resource and it now gets the vehicle name even if the ped is in it.
				if (IS_ENTITY_A_PED(entity))
				{

					// TODO Fix this part to work, I reverted this for now.
					if (IS_PED_IN_ANY_VEHICLE(entity, false))
					{
						Vehicle pedVehicle = GET_VEHICLE_PED_IS_IN(entity, false);
						// Get the vehicle name
						std::string vehicleNameString = vehicleScripts.GetVehicleName(pedVehicle);

						// Display the vehicle name instead of a model hash.
						textScripts.SetTextEntry(vehicleNameString.c_str());
						textScripts.TextPosition(entityModelMenuPosX, entityModelMenuPosY);
					}

					//// Check if the ped is in a vehicle, if not display the entity model hash.
					//if (DOES_ENTITY_EXIST(pedVehicle))
					//{
					//	// Get the vehicle name
					//	std::string vehicleNameString = vehicleScripts.GetVehicleName(pedVehicle);

					//	// Display the vehicle name instead of a model hash.
					//	textScripts.SetTextEntry(vehicleNameString.c_str());
					//	textScripts.TextPosition(entityModelMenuPosX, entityModelMenuPosY);
					//}
					// Show the entity model hash if not in a vehicle.
					else
					{

						std::string entityModelString;
						auto it = KCMenu::g_modelNames.find(entityHash);
						if (it != KCMenu::g_modelNames.end())
						{
							// Found the model name
							entityModelString = std::format("Entity Model: {}", it->second);
						}
						else
						{
							// Model name not found, fallback to displaying the hash
							entityModelString = std::format("Entity Hash: {}", std::to_string(entityHash));
						}


						// This seems to work for displaying the ped models also
						// The text isn't centered like the vehicle one though.
						// TODO Make this text match with the other ones.
						textScripts.SetTextEntry(entityModelString.c_str());
						textScripts.TextPosition(entityModelMenuPosX, entityModelMenuPosY);

						//std::string entityIdString = std::to_string(static_cast<DWORD>(entityHash));
						//std::string entityId = std::format("Entity Hash: {}", entityIdString);

						//textScripts.SetTextEntry(entityId.c_str());
						//textScripts.TextPosition(entityModelMenuPosX, entityModelMenuPosY);


					}
				}

				//------
				// If this is just a vehicle with no ped in it.
				//------
				else if (IS_ENTITY_A_VEHICLE(entity))
				{

					//GTAvehicle entityVeh = entity;

					// Get the vehicle name
					std::string vehicleNameString = vehicleScripts.GetVehicleName(entity);

					//textScripts.SetTextEntry(vehicleNameString.c_str());
					textScripts.SetTextEntry(vehicleNameString.c_str());
					textScripts.TextPosition(entityModelMenuPosX, entityModelMenuPosY);
				}
				//------
				// If this is not a ped in a vehicle or a vehicle, show a model hash
				//------
				else
				{
					//------
					//Entity Model hash or ped name/object name.
					//------
					// 


					// New method:
					// This works for getting object names instead of hashes!
					// Check if the hash exists in our map
					std::string entityModelString;
					auto it = KCMenu::g_modelNames.find(entityHash);
					if (it != KCMenu::g_modelNames.end()) {
						// Found the model name
						entityModelString = std::format("Entity Model: {}", it->second);
					}
					else {
						// Model name not found, fallback to displaying the hash
						//entityModelString = std::format("Entity Hash: {}", std::to_string(entityHash));
						entityModelString = std::format("Entity Hash: {}", std::to_string(entityHash));
					}

					// Display text on screen
					//textScripts.SetTextEntry(entityModelString.c_str(), 255, 255, 255, 255);
					textScripts.SetTextEntry(entityModelString.c_str());
					textScripts.TextPosition(entityModelMenuPosX, entityModelMenuPosY);


					// Original method:
					//std::string entityModelString = std::format("Entity Hash: {}", std::to_string(entityHash));
					//textScripts.SetTextEntry(entityModelString.c_str(), 255, 255, 255, 255);
					//textScripts.TextPosition(entityModelMenuPosX, entityModelMenuPosY);

					// Possibly useful for logging these, although like this it spams the console.
					//log_output(entityModelString);

					// TODO Add door status if this is a door, I would probably have to check if this is a door model from a list.

				}
			}
		}

		//void DrawInfos()
		//{
		//	/*
		//	    local args = {...}
		//    for k,v in pairs(args) do
		//	*/
		//	float ypos = 0.70;
		//
		//	SET_TEXT_COLOUR(255, 255, 255, 255);
		//	SET_TEXT_FONT(0);
		//	SET_TEXT_SCALE(0.4, 0.4);
		//	SET_TEXT_WRAP(0.0, 1.0);
		//	SET_TEXT_CENTRE(false);
		//	SET_TEXT_DROPSHADOW(0, 0, 0, 0, 255);
		//	SET_TEXT_EDGE(50, 0, 0, 0, 255);
		//	SET_TEXT_OUTLINE();
		//	//SetTextEntry("STRING");
		//	//AddTextComponentString(v);
		//	//DRAW_TEXT(0.015, ypos);
		//	ypos = ypos + 0.028;
		//}

		// End adapted from pun_idgun FiveM

	} // namespace IDGun
}