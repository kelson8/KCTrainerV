## Natives to look into:

I am getting this list of natives from here:
https://nativedb.dotindustries.dev/gta5/natives/

Also, my natives header is using compact vectors, so most functions instead of taking an x, y, z, they are taking a Vector3.

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
