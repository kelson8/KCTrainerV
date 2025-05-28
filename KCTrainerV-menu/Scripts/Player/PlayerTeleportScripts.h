#pragma once
#include <inc/types.h>

#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

#include "Util/Enums.h"

#include <inc/types.h>


namespace Scripts
{
	namespace Player
	{
		class Positions
		{
		public:
			// Instance of Positions
			static Positions& GetInstance()
			{
				static Positions instance;
				return instance;
			}

			// Teleports
			// This is in use for the teleport function
			// 
			// Just set the players position here.
			void SetPlayerCoords(Vector3 position);
			// Set the player position, heading, and fade if true.
			void SetPlayerCoords(Vector3 position, float heading, bool fade);

			void SetPlayerHeading(float heading);

			// Get the waypoint coords
			Vector3 GetWaypointCoords();

			//

			// Get player coords and heading
			Vector3 GetPlayerCoords();
			float GetPlayerHeading();
			//

		private:
			Positions() {} // Private constructor to prevent external instantiation
			Positions(const Positions&) = delete;
			Positions& operator=(const Positions&) = delete;

			// Will this format work?
			//PlayerScripts& playerScripts = PlayerScripts::GetInstance();
			
			// I normally use auto
			//auto& playerScripts = PlayerScripts::GetInstance();


		};
	} // namespace Player
} // namespace Scripts

