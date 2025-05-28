#include "pch.h"

#include "PlayerTeleportScripts.h"

#include "Constants.hpp"

#include "common.h"

#include "Util/Hash.h"

// My scripts

#include "Scripts/PlayerScripts.h"
#include "Scripts/PedScripts.h"

#include "Scripts/Stats.h"
#include "Scripts/MiscScripts.h"

namespace Scripts 
{

    namespace Player
    {
        /// <summary>
        /// Get the players current coords
        /// </summary>
        /// <returns>Players current coords as a vector3</returns>
        Vector3 Positions::GetPlayerCoords()
        {
            auto& playerScripts = PlayerScripts::GetInstance();
            Vector3 playerCoords = GET_ENTITY_COORDS(playerScripts.GetPlayerPed(), false);
            return playerCoords;
        }

        /// <summary>
        /// Get the players heading
        /// </summary>
        /// <returns>The players heading as a float</returns>
        float Positions::GetPlayerHeading()
        {
            auto& playerScripts = PlayerScripts::GetInstance();
            float playerHeading = GET_ENTITY_HEADING(playerScripts.GetPlayerPed());
            return playerHeading;
        }

        /// <summary>
        /// Teleport player to the specified coords, and a heading
        /// I added extra code in this from the Chaos Mod in Player.h.
        /// Now this should teleport the players vehicle, and possibly make them not fall through the ground.
        /// TODO Fix this, sometimes crashes and doesn't get the ground coordinate.
        /// </summary>
        /// <param name="position"></param>
        void Positions::SetPlayerCoords(Vector3 position)
        {
            auto& playerScripts = PlayerScripts::GetInstance();
            // Based on teleport implementation in Chaos Mod (Player.h)
            auto playerPed = playerScripts.GetPlayerPed();
            bool isInVeh = IS_PED_IN_ANY_VEHICLE(playerPed, false);
            bool isInFlyingVeh = IS_PED_IN_FLYING_VEHICLE(playerPed);
            auto playerVeh = GET_VEHICLE_PED_IS_IN(playerPed, false);
            // TODO Figure out what exactly the velocity is used for 
            auto vel = GET_ENTITY_VELOCITY(isInVeh ? playerVeh : playerPed);
            float groundHeight = GET_ENTITY_HEIGHT_ABOVE_GROUND(playerVeh);

            // Forward speed for the vehicle
            float forwardSpeed;

            int fadeOutTime = 1000;
            int fadeInTime = 1000;

            // If the player is in the vehicle, this should store their forwardSpeed
            if (isInVeh)
            {
                forwardSpeed = GET_ENTITY_SPEED(playerVeh);
                //log_output(std::format("Forward speed: {}", forwardSpeed));
            }

            // TODO Test without this, Menyoo and my FiveM scripts isn't doing this.
            //STREAMING::LOAD_SCENE(position);

            // New addition for this checks if the player is in a vehicle, if so it also teleports the vehicle.
            // And it checks if the player is in a flying vehicle.
            SET_ENTITY_COORDS(isInVeh ? playerVeh : playerPed,
                Vector3(position.x, position.y, isInFlyingVeh ? position.z + groundHeight : position.z), false, false, false, false);

            // TODO Figure out what this part does
            //SET_ENTITY_VELOCITY(isInVeh ? playerVeh : playerPed, Vector3(vel.x, vel.y, vel.z));

            // Set the vehicle to the speed it previously was, this doesn't seem to work.
            // Actually this works for a second then stops
            if (isInVeh)
            {
                SET_VEHICLE_FORWARD_SPEED(playerVeh, forwardSpeed);
                //log_output(std::format("Forward speed set to: {}", forwardSpeed));
            }
        }

        /// <summary>
        /// Set the player coords, this function also sets the heading, and fades if enabled.
        /// </summary>
        /// <param name="position"></param>
        /// <param name="heading"></param>
        /// <param name="fade"></param>
        void Positions::SetPlayerCoords(Vector3 position, float heading, bool fade)
        {
            // Based on teleport implementation in Chaos Mod (Player.h)
            auto& playerScripts = PlayerScripts::GetInstance();
            auto playerPed = playerScripts.GetPlayerPed();
            bool isInVeh = IS_PED_IN_ANY_VEHICLE(playerPed, false);
            bool isInFlyingVeh = IS_PED_IN_FLYING_VEHICLE(playerPed);
            auto playerVeh = GET_VEHICLE_PED_IS_IN(playerPed, false);
            // TODO Figure out what exactly the velocity is used for 
            auto vel = GET_ENTITY_VELOCITY(isInVeh ? playerVeh : playerPed);
            float groundHeight = GET_ENTITY_HEIGHT_ABOVE_GROUND(playerVeh);

            // Forward speed for the vehicle, disabled, this needs a option in the menu.
            //float forwardSpeed;

            //// If the player is in the vehicle, this should store their forwardSpeed
            //if (isInVeh)
            //{
            //    forwardSpeed = GET_ENTITY_SPEED(playerVeh);
            //    //log_output(std::format("Forward speed: {}", forwardSpeed));
            //}

            int fadeOutTime = 1000;
            int fadeInTime = 1000;

            // Fade out
            if (fade)
            {
                playerScripts.FadeScreenOut(fadeOutTime);
            }

            // Set coords
            SET_ENTITY_COORDS(isInVeh ? playerVeh : playerPed,
                Vector3(position.x, position.y, isInFlyingVeh ? position.z + groundHeight : position.z), false, false, false, false);

            // Set heading
            SET_ENTITY_HEADING(isInVeh ? playerVeh : playerPed, heading);

            // Set the vehicle to the speed it previously was
            // Well this works but it's not a good idea for tight spaces lol.
            // TODO Add a bool option in the vehicle options menu.
            //if (isInVeh)
            //{
            //    SET_VEHICLE_FORWARD_SPEED(playerVeh, forwardSpeed);
            //    //log_output(std::format("Forward speed set to: {}", forwardSpeed));
            //}

            // Fade in
            if (fade)
            {
                playerScripts.FadeScreenIn(fadeInTime);
            }
        }

        /// <summary>
        /// Set the players heading.
        /// </summary>
        /// <param name="heading"></param>
        void Positions::SetPlayerHeading(float heading)
        {
            auto& playerScripts = PlayerScripts::GetInstance();
            ENTITY::SET_ENTITY_HEADING(playerScripts.GetPlayerPed(), heading);
        }

        /// <summary>
        /// Get the current waypoint coordinates if it exists.
        /// Taken from my KCTestScripts implementation in Menyoo.
        /// </summary>
        /// <returns>A Vector3 of the coords, such as this: Vector3(22.5, 15.2, 30.2)</returns>
        Vector3 Positions::GetWaypointCoords()
        {
            Vector3 coords;
            if (IS_WAYPOINT_ACTIVE())
            {
                coords = GET_BLIP_COORDS(GET_FIRST_BLIP_INFO_ID(8));
                return coords;
            }

            // Return all 0's if no waypoint found, TODO Setup check for this value.
            return Vector3(0, 0, 0);
        }

#pragma endregion

    } // namespace Player

} //namespace Scripts
