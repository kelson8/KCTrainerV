# TODO For project
1. Add refresh menu key, for reloading the config without unloading the scripts.
2. Add better organization for teleports and categories. 
3. Add more vehicles to vehicle spawner menu.
4. Add vehicle customization menu.
5. Add saving/loading menu toggles/settings such as god mode.

## Progress
Adding refresh menu key:
* I started this, haven't commited the changes yet
 and I'm not sure how exactly to do this just yet. 
There doesn't seem to be a way to easily reload the config.

Adding better organazation: 
* I have moved most code into their own menu files to make this project easier to navigate.

Adding more vehicles to the vehicle spawner:
* I have fixed this to now have categories, so far it just has sports, and super.
The only vehicles in this so far are the Cheetah and Scramjet.

Adding a vehicle customization menu:
* I haven't started on this just yet, I will eventually work on this.

Adding saving/loading menu toggles/settings for persistent changes:
* This needs worked on, possibly make this load from the 'settings_menu.ini'
in my mod, or make this have a new file such as 'KCTrainerPersistentSettings.ini',
come up with a better name for it.


## Look into
Using this native for drawing markers and setting up teleports:
* https://nativedb.dotindustries.dev/gta5/natives/0x28477EC23D892089?search=DRAW_MARKER

Check if player/entity is in area:
* https://nativedb.dotindustries.dev/gta5/natives/0x54736AA40E271165?search=IS_ENTITY_IN_AREA

* Creating teleport markers, replicate setup from FiveM.
* Creating checkpoints like in races.
* Creating coronas.