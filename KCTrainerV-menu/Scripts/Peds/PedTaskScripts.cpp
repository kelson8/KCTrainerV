#include "pch.h"

#include "PedTaskScripts.h"

#include "Constants.hpp"

#include "common.h"

#include "Util/Hash.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/PedScripts.h"

#include "Scripts/Stats.h"
#include "Scripts/MiscScripts.h"

namespace Scripts 
{

    namespace Ped
    {

		// All these TaskFunctions below are untested.
#pragma region TaskFunctions

#pragma region VehicleTasks

		// TODO Make change the name for this
		GTAPed Tasks::ped = 0;

		/// <summary>
		/// Set the ped into a vehicle
		/// TODO What's the difference in this and the below function?
		/// </summary>
		/// <param name="ped"></param>
		/// <param name="vehicle"></param>
		/// <param name="seatIndex"></param>
		void Tasks::SetPedIntoVehicle(GTAPed ped, Vehicle vehicle, int seatIndex)
		{
			// First, check if the ped and vehicle exist, TODO Make a function for this later
			if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
			{
				SET_PED_INTO_VEHICLE(ped, vehicle, seatIndex);
			}
		}

		/// <summary>
		/// Warp the ped into a vehicle
		/// </summary>
		/// <param name="ped"></param>
		/// <param name="vehicle"></param>
		/// <param name="seat"></param>
		void Tasks::WarpPedIntoVehicle(GTAPed ped, Vehicle vehicle, int seat)
		{
			// First, check if the ped and vehicle exist
			if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
			{
				TASK_WARP_PED_INTO_VEHICLE(ped, vehicle, seat);
			}
		}

		/// <summary>
		/// Enter a vehicle instead of warping directly into one.
		/// </summary>
		/// <param name="ped"></param>
		/// <param name="vehicle"></param>
		/// <param name="timeout"></param>
		/// <param name="seat"></param>
		/// <param name="speed"></param>
		/// <param name="flag"></param>
		/// <param name="overrideEntryClipsetName"></param>
		void Tasks::EnterVehicle(GTAPed ped, Vehicle vehicle, int timeout, int seat, float speed, int flag, const char* overrideEntryClipsetName)
		{
			if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
			{
				TASK_ENTER_VEHICLE(ped, vehicle, timeout, seat, speed, flag, overrideEntryClipsetName);
			}
		}

		/// <summary>
		/// Leave the vehicle
		/// </summary>
		/// <param name="ped"></param>
		/// <param name="vehicle"></param>
		/// <param name="flags"></param>
		void Tasks::LeaveVehicle(GTAPed ped, Vehicle vehicle, int flags)
		{
			// First, check if the ped and vehicle exist, TODO Make a function for this later
			if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
			{
				TASK_LEAVE_VEHICLE(ped, vehicle, flags);
			}
		}

		/// <summary>
		/// Drive to a coord
		/// </summary>
		/// <param name="ped"></param>
		/// <param name="vehicle"></param>
		/// <param name="pos"></param>
		/// <param name="speed"></param>
		/// <param name="p6"></param>
		/// <param name="vehicleModel"></param>
		/// <param name="drivingMode"></param>
		/// <param name="stopRange"></param>
		/// <param name="straightLineDistance"></param>
		void Tasks::DriveToCoord(GTAPed ped, Vehicle vehicle, Vector3 pos, float speed, Any p6, Hash vehicleModel,
			int drivingMode, float stopRange, float straightLineDistance)
		{
			if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
			{
				TASK_VEHICLE_DRIVE_TO_COORD(ped, vehicle, pos, speed, p6, vehicleModel, drivingMode, stopRange, straightLineDistance);
			}
		}

		/// <summary>
		/// Drive to a coord long range
		/// </summary>
		/// <param name="ped"></param>
		/// <param name="vehicle"></param>
		/// <param name="pos"></param>
		/// <param name="speed"></param>
		/// <param name="driveMode"></param>
		/// <param name="stopRange"></param>
		void Tasks::DriveToCoordLongRange(GTAPed ped, Vehicle vehicle, Vector3 pos, float speed, int driveMode, float stopRange)
		{
			if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
			{
				TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(ped, vehicle, pos, speed, driveMode, stopRange);
			}
		}

		/// <summary>
		/// Drive and wander around
		/// </summary>
		/// <param name="ped"></param>
		/// <param name="vehicle"></param>
		/// <param name="speed"></param>
		/// <param name="drivingStyle"></param>
		void Tasks::DriveWander(GTAPed ped, Vehicle vehicle, float speed, int drivingStyle)
		{
			if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
			{
				TASK_VEHICLE_DRIVE_WANDER(ped, vehicle, speed, drivingStyle);
			}
		}

		/// <summary>
		/// Find a spot to park the vehicle.
		/// </summary>
		/// <param name="ped"></param>
		/// <param name="vehicle"></param>
		/// <param name="pos"></param>
		/// <param name="heading"></param>
		/// <param name="mode"></param>
		/// <param name="radius"></param>
		/// <param name="keepEngineOn"></param>
		void Tasks::ParkVehicle(GTAPed ped, Vehicle vehicle, Vector3 pos, float heading, int mode, float radius, bool keepEngineOn)
		{
			if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
			{
				TASK_VEHICLE_PARK(ped, vehicle, pos, heading, mode, radius, keepEngineOn);
			}
		}

		void Tasks::SetDrivingStyle(GTAPed ped, int drivingStyle)
		{
			if (DOES_ENTITY_EXIST(ped))
			{
				SET_DRIVE_TASK_DRIVING_STYLE(ped, drivingStyle);
			}
		}

		void Tasks::SetCruiseSpeed(GTAPed driver, float cruiseSpeed)
		{
			if (DOES_ENTITY_EXIST(ped))
			{
				SET_DRIVE_TASK_CRUISE_SPEED(driver, cruiseSpeed);
			}

		}

		void Tasks::SetMaxCruiseSpeed(GTAPed ped, float speed)
		{
			if (DOES_ENTITY_EXIST(ped))
			{
				SET_DRIVE_TASK_MAX_CRUISE_SPEED(ped, speed);
			}
		}

#pragma endregion // VehicleTasks



#pragma endregion // TaskFunctions
    } // Ped

} //namespace Scripts
