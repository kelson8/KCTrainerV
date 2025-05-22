#include "VehicleOptionsMenu.h"

#include "pch.h"
#include "Constants.hpp"

#include "Scripts/Extras/Game.h"

#include "Util/UI.hpp"

#include "Util/Hash.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/VehicleScripts.h"
#include "Scripts/VehicleSpawner.h"

#include "Scripts/Vehicles/VehicleData.h"

void VehicleOptionsMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Vehicle Options");

    auto& vehicleScripts = VehicleScripts::GetInstance();
    auto& vehicleSpawner = VehicleSpawner::GetInstance();

    //--------------
    // Vehicle Options
    //--------------

    // This didn't seem to do anything
    //mbCtx.drawInstructionalButtons();

    mbCtx.BoolOption("Toggle Invincible vehicle", vehicleScripts.isInvincibleVehicleEnabled, { "Toggle invincibility for your current vehicle" });

    if (mbCtx.Option("Repair vehicle", { "Fully fix your vehicle and tires." }))
    {
        vehicleScripts.RepairVehicle();
    }

    //-----
    // Bullet proof tires toggle
    //-----
    mbCtx.BoolOption("Bulletproof tires", vehicleScripts.isBulletProofEnabled, { "Set vehicle to have bulletproof tires" });


    //-----
    // Vehicle armor options
    //-----
    mbCtx.IntOption("Vehicle Armor Level", vehicleScripts.vehicleArmorLevel, 0, 4, 1, { "Armor level selector" });
    if (mbCtx.Option("Set armor"))
    {
        vehicleScripts.SetArmor(vehicleScripts.vehicleArmorLevel);
    }

    //--------------

}