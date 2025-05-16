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

function teleport_player(x, y, z)
  local playerPed = GetPlayerPedId()
  SetEntityCoords(playerPed, x, y, z, false, false, false, true)
end

-- You can define teleport locations in Lua as well
teleport_locations = {
  airport = {-1336.0, -3044.0, 13.9},
  house = {7.11903, 536.615, 176.028}
  -- Add more locations
}

function teleport_to_location(location_name)
  local location = teleport_locations[location_name]
  if location then
    teleport_player(location[1], location[2], location[3])
    print("Teleported to " .. location_name)
  else
    print("Teleport location '" .. location_name .. "' not found.")
  end
end