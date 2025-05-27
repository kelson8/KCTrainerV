#include "pch.h"

#include "WeaponCategoryMenu.h"

#include "Constants.hpp"

#include "common.h"

#include "Util/Hash.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/PedScripts.h"

#include "Scripts/Stats.h"
#include "Scripts/MiscScripts.h"

#include "Script.hpp"
#include "Util/Util.hpp"

namespace Menus 
{
    namespace Weapon
    {
        /// <summary>
        /// Build the weapon category submenu.
        /// </summary>
        /// <param name="mbCtx"></param>
        /// <param name="context"></param>
        void CategoryMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            mbCtx.Title("Weapons");

            // This works!
            mbCtx.MenuOption("Melee", "MeleeWeaponsSubmenu");
            mbCtx.MenuOption("Handguns", "HandgunWeaponsSubmenu");
            mbCtx.MenuOption("SMG", "SmgWeaponsSubmenu");
            mbCtx.MenuOption("Shotguns", "ShotgunWeaponsSubmenu");

            mbCtx.MenuOption("Assault Rifles", "AssaultRifleWeaponsSubmenu");
            mbCtx.MenuOption("Machine Guns", "MachineGunsWeaponsSubmenu");
            mbCtx.MenuOption("Sniper Rifles", "SniperRiflesWeaponsSubmenu");
            mbCtx.MenuOption("Heavy Weapons", "HeavyWeaponsSubmenu");

            mbCtx.MenuOption("Throwables", "ThrowableWeaponsSubmenu");
            mbCtx.MenuOption("Misc", "MiscWeaponsSubmenu");
        }

        /// <summary>
        /// Build all weapon submenus
        /// To use in the KCMainMenu:
        /// Under a subcategory: 'playerMenu.BuildWeaponCategorySubmenu(mbCtx, context, "melee"); // Pass category key'
        /// </summary>
        /// <param name="mbCtx"></param>
        /// <param name="context"></param>
        /// <param name="categoryKey">The category key from the weapons json file, such as 'heavy_weapons'</param>
        void CategoryMenu::BuildWeaponCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context, const std::string& categoryKey)
        {
            // Look up the actual weapons data using the categoryKey
            auto it = KCMenu::g_weaponCategories.find(categoryKey);
            if (it == KCMenu::g_weaponCategories.end())
            {
                LOG(ERROR, std::format("BuildWeaponCategorySubmenu: Category '{}' not found in g_weaponCategories!", categoryKey.c_str()));
                mbCtx.Title("Error: " + categoryKey); // Display an error title
                mbCtx.Option("No weapons found.");
                return;
            }

            // Make this get the weapon name from the category display, instead of displaying the json string for the titles.
            const std::string categoryDisplayName = Util::GetWeaponCategoryDisplayName(categoryKey);

            const std::vector<WeaponInfo>& weaponsInThisCategory = it->second;

            // --- Logs ---
            //LOG(DEBUG, std::format("Submenu Build: Entering {} submenu (key: {})", categoryDisplayName.c_str(), categoryKey.c_str()));
            mbCtx.Title(categoryDisplayName); // Set submenu title
            //LOG(DEBUG, std::format("Submenu Build: Title set to '{}'.", categoryDisplayName.c_str()));

            if (weaponsInThisCategory.empty())
            {
                LOG(WARN, std::format("Submenu Build: WARNING - 'weaponsInThisCategory' is EMPTY for category '{}'!", categoryDisplayName.c_str()));
                mbCtx.Option("No weapons in this category."); // Display a message if empty
            }
            else
            {
                //LOG(DEBUG, std::format("Submenu Build: Category '{}' has {} weapons to display.", categoryDisplayName.c_str(),
                    //(int)weaponsInThisCategory.size()));
            }

            for (const auto& weaponInfo : weaponsInThisCategory)
            {
                //LOG(DEBUG, std::format("Submenu Build: Attempting to add option for weapon: '{}' (Hash: 0x{})",
                    //weaponInfo.name.c_str(), weaponInfo.hash));
                //if (weaponInfo.name.empty())
                //if (weaponInfo.name.empty())
                //{
                //    //LOG(WARN, std::format("Submenu Build: WARNING - Weapon name is empty for hash 0x{} in category {}.",
                //        //weaponInfo.hash, categoryDisplayName.c_str()));
                //}

                //if (mbCtx.Option(weaponInfo.name))
                if (mbCtx.Option(weaponInfo.displayName))
                {
                    GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), weaponInfo.hash, 999, false, true);
                    //LOG(DEBUG, std::format("Submenu Build: Gave weapon '{}' to player.", weaponInfo.name.c_str()));
                    //mbCtx.CloseMenu(); // Optionally close menu
                }
            }
            // --- End logs ---
        }
    } //namespace Weapon

} //namespace Menus
