#pragma once
#include <inc/types.h>

#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

typedef Ped GTAPed; // Assuming Ped is the global GTA type

namespace Scripts
{
	namespace Ped
	{
		class Tasks
		{
		public:
			// Instance of Tasks
			static Tasks& GetInstance()
			{
				static Tasks instance;
				return instance;
			}

			
			// Vehicle
			void SetPedIntoVehicle(GTAPed ped, Vehicle vehicle, int seatIndex);
			void WarpPedIntoVehicle(GTAPed ped, Vehicle vehicle, int seat);

			void EnterVehicle(GTAPed ped, Vehicle vehicle, int timeout, int seat, float speed, int flag, const char* overrideEntryClipsetName);
			void LeaveVehicle(GTAPed ped, Vehicle vehicle, int flags);

			void DriveToCoord(GTAPed ped, Vehicle vehicle, Vector3 pos, float speed, Any p6, Hash vehicleModel,
				int drivingMode, float stopRange, float straightLineDistance);

			void DriveToCoordLongRange(GTAPed ped, Vehicle vehicle, Vector3 pos, float speed, int driveMode, float stopRange);
			void DriveWander(GTAPed ped, Vehicle vehicle, float speed, int drivingStyle);

			void ParkVehicle(GTAPed ped, Vehicle vehicle, Vector3 pos, float heading, int mode, float radius, bool keepEngineOn);
			void SetDrivingStyle(GTAPed ped, int drivingStyle);

			void SetCruiseSpeed(GTAPed driver, float cruiseSpeed);
			void SetMaxCruiseSpeed(GTAPed ped, float speed);
			//
		private:
			Tasks() {} // Private constructor to prevent external instantiation
			Tasks(const Tasks&) = delete;
			Tasks& operator=(const Tasks&) = delete;

			static GTAPed ped;

		};
	} // namespace Ped
} // namespace Scripts

