-- scripts/kctrainer-v.lua


-- GetEntityCoords (returns a Vector3)
-- local coords = _invoke(GetHashKey("GET_ENTITY_COORDS"), 6, PlayerPedId()) -- Assuming 6 is your Vector3 return type

-- if coords then
--   print("X:", coords.X, "Y:", coords.Y, "Z:", coords.Z)
-- end

-- -- TriggerMusicEvent (void return, so a dummy return type like 0 might be used)
-- _invoke(GetHashKey("TRIGGER_MUSIC_EVENT"), 0, "SHOP_TILL_YOU_DROP")

-- -- GetPlayerPedId (returns an int)
-- local playerPed = _invoke(GetHashKey("PLAYER_PED_ID"), 1) -- Assuming 1 is your Int return type
-- print("Player Ped:", playerPed)


-- TODO Fix this later
-- New
-- Define return types (as in previous example)
-- RETURN_BOOL = 0
-- RETURN_INT = 1
-- RETURN_UINT = 2
-- RETURN_FLOAT = 3
-- RETURN_STRING = 4
-- RETURN_VECTOR3 = 5
-- RETURN_VOID = 6

-- -- Create a wrapper function for cleaner calls
-- function invoke(nativeName, returnType, ...)
--     local nativeHash = GET_NATIVE_HASH(nativeName)
--     if nativeHash == 0 then
--         -- Handle the error in Lua if the native name wasn't found
--         print("ERROR: Native '" .. nativeName .. "' not found!")
--         return nil
--     end
--     return _invoke(nativeHash, returnType, ...)
-- end
--

-- Example Usage:
-- local playerPed = invoke("GET_PLAYER_PED", RETURN_INT, -1)
-- invoke("SET_ENTITY_COORDS", RETURN_VOID, playerPed, 123.45, 678.90, 10.0, false, false, false, true)



-- function teleport_player(x, y, z)
function teleport_player(pos)
  local playerPed = GetPlayerPedId()
  -- local targetPosition = Vector3.new(x, y, z)
  
  -- SetEntityCoords(playerPed, x, y, z, false, false, false, true)
  SetEntityCoords(playerPed, pos, false, false, false, false)
end

-- You can define teleport locations in Lua as well
teleport_locations = {
  airport = {-1336.0, -3044.0, 13.9},
  house = {7.11903, 536.615, 176.028}
  -- Add more locations
}

function teleport_to_location(location_name)
  local location_data = teleport_locations[location_name]
  if location_data then
	local target_position = Vector3.new(location_data[1], location_data[2], location_data[3])
    teleport_player(target_position)
    print("Teleported to " .. location_name)
  else
    print("Teleport location '" .. location_name .. "' not found.")
  end
end

-- This will get the teleport location names.
function get_teleport_location_names()
  local names = {}
  for name, _ in pairs(teleport_locations) do
    table.insert(names, name)
  end
  return names
end