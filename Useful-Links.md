# Useful links for GTA V modding/scripting:

This is a list of useful natives, vehicle models ids, ped model ids, and more 
mostly for use with GTA V ScriptHookV.

I have teleport locations, outfit lists for the multiplayer peds, 
and more in the list below.

## Data/Teleports
* Useful project for looking into doors and interiors: https://github.com/SuleMareVientu/InteriorsV-ScriptHookV
* GTA V Data Dumps: https://github.com/DurtyFree/gta-v-data-dumps/tree/master
* List of a bunch of teleports: https://gtaforums.com/topic/792877-list-of-over-100-coordinates-more-comming/
* List of IPLs and locations: https://wiki.rage.mp/wiki/Interiors_and_Locations
* Character Model outfit list: https://wiki.rage.mp/wiki/Player::setComponentVariation
* List of outfit examples for C++: https://gist.github.com/root-cause/e77c1b1e003402687291ea76b0187677#file-outfitvariantexample-cpp

### Models:
* Vehicle Model list: https://docs.fivem.net/docs/game-references/vehicle-references/vehicle-models/
* Vehicle Colors: https://docs.fivem.net/docs/game-references/vehicle-references/vehicle-colours/
* Vehicle Flags: https://docs.fivem.net/docs/game-references/vehicle-references/vehicle-flags/
* Ped Models: https://docs.fivem.net/docs/game-references/ped-models/
* Weapon Models & Components: https://docs.fivem.net/docs/game-references/weapon-models/

### Misc
* Controls: https://docs.fivem.net/docs/game-references/controls/
* Input mapper IDs: https://docs.fivem.net/docs/game-references/input-mapper-parameter-ids/
* Blips: https://docs.fivem.net/docs/game-references/blips/
* Checkpoints: https://docs.fivem.net/docs/game-references/checkpoints/
* Markers: https://docs.fivem.net/docs/game-references/markers/
* Data files: https://docs.fivem.net/docs/game-references/data-files/
* Draw instructional buttons (Lua guide): https://docs.fivem.net/docs/game-references/instructional-buttons/
* HUD Colors: https://docs.fivem.net/docs/game-references/hud-colors/
* Pickup Hashes: https://docs.fivem.net/docs/game-references/pickup-hashes/
* Profile Settings: https://docs.fivem.net/docs/game-references/profile-settings/
* Radio Stations: https://docs.fivem.net/docs/game-references/radiostations/
* Text formatting: https://docs.fivem.net/docs/game-references/text-formatting/
* Speeches: https://docs.fivem.net/docs/game-references/speeches/

# Natives:
* https://nativedb.dotindustries.dev/gta5/natives/
* https://docs.fivem.net/natives

### Create items
* Create Pickups: https://docs.fivem.net/natives/?_0xFBA08C503DD5FA58
* Create Objects: https://docs.fivem.net/natives/?_0x509D5878EB39E842
* Draw Corona: https://docs.fivem.net/natives/?_0xFF44780E

### Cellphone
* CELL_CAM_ACTIVATE: https://nativedb.dotindustries.dev/gta5/natives/0xFDE8F069C542D126

### Radar/Hud
* SET_RADAR_ZOOM: https://nativedb.dotindustries.dev/gta5/natives/0x096EF57A0C999BBA
* SET_USE_SET_DESTINATION_IN_PAUSE_MAP: https://nativedb.dotindustries.dev/gta5/natives/0x6CDD58146A436083
* SET_FORCE_SHOW_GPS: https://nativedb.dotindustries.dev/gta5/natives/0x2790F4B17D098E26
* DISPLAY_HUD: https://nativedb.dotindustries.dev/gta5/natives/0xA6294919E56FF02A

### Text
* GET_FIRST_N_CHARACTERS_OF_LITERAL_STRING: https://nativedb.dotindustries.dev/gta5/natives/0x98C3CF913D895111
* DOES_TEXT_BLOCK_EXIST: https://nativedb.dotindustries.dev/gta5/natives/0x1C7302E725259789
* CLEAR_ALL_HELP_MESSAGES: https://nativedb.dotindustries.dev/gta5/natives/0x6178F68A87A4D3A0

### Ped clothes
* SET_PED_COMPONENT_VARIATION: https://nativedb.dotindustries.dev/gta5/natives/0x262B14F48D29DE80?search=component_var
* IS_PED_COMPONENT_VARIATION_VALID: https://nativedb.dotindustries.dev/gta5/natives/0xE825F6B6CEA7671D?search=component_var
* SET_PED_RANDOM_COMPONENT_VARIATION: https://nativedb.dotindustries.dev/gta5/natives/0xC8A9481A01E63C28?search=component_var
* SET_PED_DEFAULT_COMPONENT_VARIATION: https://nativedb.dotindustries.dev/gta5/natives/0x45EEE61580806D63?search=component_var
* GET_TINT_INDEX_FOR_LAST_GEN_HAIR_TEXTURE: https://nativedb.dotindustries.dev/gta5/natives/0xC56FBF2F228E1DAC?search=component_var

* SET_PED_HAIR_TINT: https://nativedb.dotindustries.dev/gta5/natives/0x4CFFC65454C93A49?search=SET_PED_HAIR_TINT

### Weapon
* GIVE_WEAPON_TO_PED: https://nativedb.dotindustries.dev/gta5/natives/0xBF0FD6E56C964FCB?search=give_weapon
* GIVE_WEAPON_COMPONENT_TO_PED: https://nativedb.dotindustries.dev/gta5/natives/0xD966D51AA5B28BB9?search=give_weapon
* GIVE_WEAPON_COMPONENT_TO_WEAPON_OBJECT: https://nativedb.dotindustries.dev/gta5/natives/0x33E179436C0B31DB?search=give_weapon
* GIVE_WEAPON_OBJECT_TO_PED: https://nativedb.dotindustries.dev/gta5/natives/0xB1FA61371AF7C4B7?search=give_weapon

* GET_WEAPON_COMPONENT_VARIANT_EXTRA_COUNT: https://nativedb.dotindustries.dev/gta5/natives/0x6558AC7C17BFEF58?search=component_var
* GET_WEAPON_COMPONENT_VARIANT_EXTRA_MODEL: https://nativedb.dotindustries.dev/gta5/natives/0x4D1CB8DC40208A17?search=component_var


### Garage
* SET_VEHICLE_CAN_SAVE_IN_GARAGE: https://nativedb.dotindustries.dev/gta5/natives/0x428BACCDF5E26EAD?search=garage
* STOP_ALL_GARAGE_ACTIVITY: https://nativedb.dotindustries.dev/gta5/natives/0x0F87E938BDF29D66?search=garage
* IS_GARAGE_EMPTY: https://nativedb.dotindustries.dev/gta5/natives/0x90E47239EA1980B8?search=garage
* CLEAR_GARAGE: https://nativedb.dotindustries.dev/gta5/natives/0xDA05194260CDCDF9?search=garage
* ENABLE_SAVING_IN_GARAGE: https://nativedb.dotindustries.dev/gta5/natives/0xF2E1A7133DD356A6?search=garage
