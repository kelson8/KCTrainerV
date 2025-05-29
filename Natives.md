## Natives to look into:

I am getting this list of natives from here:
https://nativedb.dotindustries.dev/gta5/natives/

Also, my natives header is using compact vectors, so most functions instead of taking an x, y, z, they are taking a Vector3.

<details>
<summary> Big list of natives </summary>

---
Door locks and rolling up/down windows:

---

https://nativedb.dotindustries.dev/gta5/natives/0x7C65DAC73C35C862?search=SET_VEHICLE_DOOR_OPEN
* SET_VEHICLE_DOOR_OPEN(Vehicle vehicle, int doorId, bool loose, bool openInstantly)

https://nativedb.dotindustries.dev/gta5/natives/0x3B458DDB57038F08?search=SET_VEHICLE_DOOR_AUTO_LOCK
* SET_VEHICLE_DOOR_AUTO_LOCK(Vehicle vehicle, int doorId, bool toggle) - TODO Test this

https://nativedb.dotindustries.dev/gta5/natives/0x85796B0549DDE156?search=ROLL_DOWN_WINDOWS
* ROLL_DOWN_WINDOWS(Vehicle vehicle) - Roll down all the windows of the vehicle.

https://nativedb.dotindustries.dev/gta5/natives/0x7AD9E6CE657D69E3?search=ROLL_DOWN_WINDOW
List of window indexes:
* 0 = Front Left Window
* 1 = Front Right Window
* 2 = Rear Left Window
* 3 = Rear Right Window
* 4 = Front Windscreen
* 5 = Rear Windscreen
* 6 = Mid Left
* 7 = Mid Right
* 8 = Invalid
* ROLL_DOWN_WINDOW(Vehicle vehicle, int windowIndex) - Roll down the selected window of the car

https://nativedb.dotindustries.dev/gta5/natives/0x602E548F46E24D59?search=ROLL_UP_WINDOW
* ROLL_UP_WINDOW(Vehicle vehicle, int windowIndex) - Same window index as above, roll up the selected window of the car

https://nativedb.dotindustries.dev/gta5/natives/0x9E5B5E4D2CCD2259?search=SMASH_VEHICLE_WINDOW
* SMASH_VEHICLE_WINDOW(Vehicle vehicle, int windowIndex) - Smash the selected window

https://nativedb.dotindustries.dev/gta5/natives/0x772282EBEB95E682?search=FIX_VEHICLE_WINDOW
* FIX_VEHICLE_WINDOW(Vehicle vehicle, int windowIndex) - Fix the selected window.

https://nativedb.dotindustries.dev/gta5/natives/0x6D645D59FB5F5AD3?search=POP_OUT_VEHICLE_WINDSCREEN
* POP_OUT_VEHICLE_WINDSCREEN(Vehicle vehicle) - Remove the windshield from the vehicle.


---
Setting/modifying license plates:

---

https://nativedb.dotindustries.dev/gta5/natives/0x9088EB5A43FFB0A1?search=SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX

List of plate Indexes:

* Blue/White - 0
* Yellow/black - 1
* Yellow/Blue - 2
* Blue/White2 - 3
* Blue/White3 - 4
* Yankton - 5
* SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(Vehicle plateIndex) - Set the vehicle license plate type

https://nativedb.dotindustries.dev/gta5/natives/0xF11BC2DD9A3E7195?search=GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX
* GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(Vehicle vehicle) - Return the current license plate type of a vehicle

https://nativedb.dotindustries.dev/gta5/natives/0x4C4D6B2644F458CB?search=GET_NUMBER_OF_VEHICLE_NUMBER_PLATES
* GET_NUMBER_OF_VEHICLE_NUMBER_PLATES

https://nativedb.dotindustries.dev/gta5/natives/0x95A88F0B409CDA47?search=SET_VEHICLE_NUMBER_PLATE_TEXT
* SET_VEHICLE_NUMBER_PLATE_TEXT(Vehicle, const char* plateText) - Set the vehicle license plate text, max of 8 characters.


https://nativedb.dotindustries.dev/gta5/natives/0x7CE1CCB9B293020E?search=GET_VEHICLE_NUMBER_PLATE_TEXT
* GET_VEHICLE_NUMBER_PLATE_TEXT(Vehicle vehicle)

---
Vehicle control:

---

https://nativedb.dotindustries.dev/gta5/natives/0xF19D095E42D430CC?search=SET_VEHICLE_OUT_OF_CONTROL
* SET_VEHICLE_OUT_OF_CONTROL(Vehicle vehicle, bool killDriver, bool explodeOnImpact)

https://nativedb.dotindustries.dev/gta5/natives/0x260BE8F09E326A20?search=BRING_VEHICLE_TO_HALT
* BRING_VEHICLE_TO_HALT(Vehicle vehicle, float distance, int duration, bool p3)


---
These look fun to mess around with:

---

https://nativedb.dotindustries.dev/gta5/natives/0x99AD4CCCB128CBC9?search=ADD_VEHICLE_PHONE_EXPLOSIVE_DEVICE
* ADD_VEHICLE_PHONE_EXPLOSIVE_DEVICE(Vehicle vehicle)

https://nativedb.dotindustries.dev/gta5/natives/0xEF49CF0270307CBE?search=DETONATE_VEHICLE_PHONE_EXPLOSIVE_DEVICE
* DETONATE_VEHICLE_PHONE_EXPLOSIVE_DEVICE()

https://nativedb.dotindustries.dev/gta5/natives/0x6ADAABD3068C5235?search=HAS_VEHICLE_PHONE_EXPLOSIVE_DEVICE
* HAS_VEHICLE_PHONE_EXPLOSIVE_DEVICE()

https://nativedb.dotindustries.dev/gta5/natives/0xAA3F739ABDDCF21F?search=CLEAR_VEHICLE_PHONE_EXPLOSIVE_DEVICE
* CLEAR_VEHICLE_PHONE_EXPLOSIVE_DEVICE()
---

Setting/toggling ambient world items:

* SET_RANDOM_TRAINS
* SET_RANDOM_BOATS
* SET_GARBAGE_TRUCKS

---
Checking for models:

---

* IS_THIS_MODEL_A_BOAT
* IS_THIS_MODEL_A_JETSKI
* IS_THIS_MODEL_A_PLANE
* IS_THIS_MODEL_A_HELI
* IS_THIS_MODEL_A_CAR
* IS_THIS_MODEL_A_TRAIN
* IS_THIS_MODEL_A_BIKE
* IS_THIS_MODEL_A_QUADBIKE
* IS_THIS_MODEL_AN_AMPHIBIOUS_CAR
* IS_THIS_MODEL_AN_AMPHIBIOUS_QUADBIKE

---
Helicopter natives:

---

* SET_HELI_BLADES_FULL_SPEED
* SET_HELI_BLADES_SPEED

---
Misc vehicle natives:

---

* SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED
* SET_VEHICLE_HAS_UNBREAKABLE_LIGHTS
* SET_VEHICLE_RESPECTS_LOCKS_WHEN_HAS_DRIVER
* SET_VEHICLE_CAN_EJECT_PASSENGERS_IF_LOCKED
* GET_VEHICLE_DIRT_LEVEL
* SET_VEHICLE_DIRT_LEVEL
* SET_VEHICLE_ENGINE_ON
* SET_VEHICLE_UNDRIVEABLE

* SET_VEHICLE_LIGHTS
* SET_VEHICLE_HEADLIGHT_SHADOWS
* SET_VEHICLE_ALARM
* START_VEHICLE_ALARM
* IS_VEHICLE_ALARM_ACTIVATED
* SET_VEHICLE_INTERIORLIGHT
* SET_VEHICLE_LIGHT_MULTIPLIER


---

Train natives:

---

* CREATE_MISSION_TRAIN
* SWITCH_TRAIN_TRACK
* SET_TRAIN_TRACK_SPAWN_FREQUENCY
* DELETE_ALL_TRAINS
* SET_TRAIN_SPEED
* SET_TRAIN_CRUISE_SPEED
* GET_TRAIN_CARRIAGE
* IS_MISSION_TRAIN
* DELETE_MISSION_TRAIN
* SET_MISSION_TRAIN_AS_NO_LONGER_NEEDED
* SET_MISSION_TRAIN_COORDS
* IS_THIS_MODEL_A_TRAIN

---
Forklift natives:
First check if a vehicle is a forklift to run these:

---

SET_FORKLIFT_FORK_HEIGHT: 
* https://nativedb.dotindustries.dev/gta5/natives/0x37EBBF3117BD6A25?search=SET_FORKLIFT_FORK_HEIGHT
LOWER_FORKLIFT_FORKS: 
* https://nativedb.dotindustries.dev/gta5/natives/0x923A293361DF44E5?search=LOWER_FORKLIFT_FORKS

---

</details>

<details>
<summary> Chaos Mod natives to look into </summary>

* https://nativedb.dotindustries.dev/gta5/natives/0x5CEC1A84620E7D5B?search=SET_DISABLE_BREAKING
* https://nativedb.dotindustries.dev/gta5/natives/0xFD55E49555E017CF?search=SHAKE_GAMEPLAY_CAM
* https://nativedb.dotindustries.dev/gta5/natives/0x799017F9E3B10112?search=DRAW_MARKER_SPHERE

Waypoints:
* https://nativedb.dotindustries.dev/gta5/natives/0xFE43368D2AA4F2FC?search=SET_NEW_WAYPOINT
* https://nativedb.dotindustries.dev/gta5/natives/0x81FA173F170560D1?search=REFRESH_WAYPOINT

</details>

<details>
<summary> Area checks </summary>

IS_ENTITY_IN_AREA(Entity entity, Vector3 vec1_, Vector3 vec2_, BOOL p7, BOOL p8, Any p9)
* https://nativedb.dotindustries.dev/gta5/natives/0x54736AA40E271165

IS_COP_VEHICLE_IN_AREA_3D(float x1, float x2, float y1, float y2, float z1, float z2)
* https://nativedb.dotindustries.dev/gta5/natives/0x7EEF65D5F153E26A

IS_EXPLOSION_IN_AREA(int explosionType, Vector3 vec1_, Vector3 vec2_)
* https://nativedb.dotindustries.dev/gta5/natives/0x2E2EBA0EE7CED0E0

IS_EXPLOSION_ACTIVE_IN_AREA(int explosionType, Vector3 vec1_, Vector3 vec2_)
* https://nativedb.dotindustries.dev/gta5/natives/0x6070104B699B2EF4

IS_COP_PED_IN_AREA_3D(Vector3 vec1_, Vector3 vec2_)
* https://nativedb.dotindustries.dev/gta5/natives/0x16EC4839969F9F5E

IS_PED_SHOOTING_IN_AREA(Ped ped, Vector3 vec1_, Vector3 vec2_, BOOL p7, BOOL p8)
* https://nativedb.dotindustries.dev/gta5/natives/0x7E9DFE24AC1E58EF

IS_ANY_PED_SHOOTING_IN_AREA(Vector3 vec1_, Vector3 vec2_, BOOL p6, BOOL p7)
* https://nativedb.dotindustries.dev/gta5/natives/0xA0D3D71EA1086C55

TELL_GROUP_PEDS_IN_AREA_TO_ATTACK(Ped ped, Any p1, float p2, Hash hash, Any p4, Any p5)
* https://nativedb.dotindustries.dev/gta5/natives/0xAD27D957598E49E9

</details>

<details>
<summary> Paths </summary>
TODO Fill in some of these from pathfind that I find useful on the natives website.

SET_ALLOW_STREAM_PROLOGUE_NODES(BOOL toggle)
* https://nativedb.dotindustries.dev/gta5/natives/0x228E5C6AD4D74BFD

SET_ALLOW_STREAM_HEIST_ISLAND_NODES(BOOL toggle)
Activates Cayo Perico path nodes if passed `1`. GPS navigation will start working, maybe more stuff will change, not sure. It seems if you try to unload (pass `0`) when close to the island, your game might crash.
* https://nativedb.dotindustries.dev/gta5/natives/0xF74B1FFA4A15FBEA



</details>


<details>
<summary> Special vehicles </summary>

DETACH_VEHICLE_FROM_TOW_TRUCK(Vehicle towTruck, Vehicle vehicle)
* https://nativedb.dotindustries.dev/gta5/natives/0xC2DB6B6708350ED8

SET_VEHICLE_BULLDOZER_ARM_POSITION(Vehicle vehicle, float position, BOOL p2)
* https://nativedb.dotindustries.dev/gta5/natives/0xF8EBCCC96ADB9FB7

SET_​VEHICLE_​TOW_​TRUCK_​ARM_​POSITION(Vehicle vehicle, float position)
* https://nativedb.dotindustries.dev/gta5/natives/0xFE54B92A344583CA

ATTACH_​VEHICLE_​TO_​TOW_​TRUCK(Vehicle towTruck, Vehicle vehicle, BOOL rear, float hookOffsetX, float hookOffsetY, float hookOffsetZ)
* https://nativedb.dotindustries.dev/gta5/natives/0x29A16F8D621C4508

</details>





<details>
<summary> Vehicle natives </summary>

IS_​VEHICLE_​IN_​BURNOUT(Vehicle vehicle)
* https://nativedb.dotindustries.dev/gta5/natives/0x1297A88E081430EB

</details>



<details>
<summary> Taxi natives </summary>

SET_TAXI_LIGHTS(Vehicle vehicle, BOOL state)
* https://nativedb.dotindustries.dev/gta5/natives/0x598803E85E8448D9

IS_​TAXI_​LIGHT_​ON(Vehicle vehicle)
* https://nativedb.dotindustries.dev/gta5/natives/0x7504C0F113AB50FC

IS_PED_IN_ANY_TAXI(Ped ped)
* https://nativedb.dotindustries.dev/gta5/natives/0x6E575D6A898AB852

</details>
