import re

# TODO come up with a better name for this file.

def convert_menyoo_to_my_format(menyoo_data):
    """
    Converts a list of Menyoo-style teleport locations to your TeleportInfo format.

    Args:
        menyoo_data: A list of strings or tuples representing Menyoo teleport locations.

    Returns:
        A list of dictionaries representing your TeleportInfo format.
    """
    my_format_list = []
    for item in menyoo_data:
        if isinstance(item, tuple) and len(item) >= 4:
            # Handle the tuple format
            name = item[0]
            try:
                x = float(item[1])
                y = float(item[2])
                z = float(item[3])
                ipls = list(item[4]) if len(item) >= 5 and isinstance(item[4], (list, tuple)) else []
                my_format_list.append({
                    "name": name,
                    "coordinates": {"x": x, "y": y, "z": z},
                    "heading": 0.0,  # Default heading
                    "iplsToLoad": [ipl.replace('_sv', '') for ipl in ipls]
                })
            except ValueError:
                print(f"Error converting coordinates for tuple: {name}")
        elif isinstance(item, str):
            # Handle the TeleLocation string format using regex
            match = re.match(r'TeleLocation\("([^"]*)",\s*([-.\d]+)f?,\s*([-.\d]+)f?,\s*([-.\d]+)f?(?:,\s*\{([^}]*)\})?(?:,\s*\{[^}]*\})?\)', item)
            if match:
                name = match.group(1)
                try:
                    x = float(match.group(2))
                    y = float(match.group(3))
                    z = float(match.group(4))
                    ipls_str = match.group(5)
                    ipls = []
                    if ipls_str:
                        ipls = [ipl.strip().strip('"').replace('_sv', '') for ipl in ipls_str.split(',')]
                    my_format_list.append({
                        "name": name,
                        "coordinates": {"x": x, "y": y, "z": z},
                        "heading": 0.0,  # Default heading
                        "iplsToLoad": ipls
                    })
                except ValueError:
                    print(f"Error converting coordinates for string: {name}")
            else:
                print(f"Skipping unrecognized format: {item}")
        else:
            print(f"Skipping unrecognized item type: {type(item)}")
    return my_format_list

# Menyoo data (as a list of strings and tuples)
menyoo_data = [
    'TeleLocation("Franklin\'s House (New)", 7.11903f, 536.615f, 176.028f,{ "v_franklinshouse"_sv, "unlocked"_sv },{ "locked"_sv})',
    'TeleLocation("Franklin\'s House (Old)", -14.3803f, -1438.51f, 31.1073f)',
    'TeleLocation("Trevor\'s House", 1972.61f, 3817.04f, 33.4278f)',
    'TeleLocation("Trevor\'s House 2", -1151.77f, -1518.14f, 10.6327f)',
    'TeleLocation("Trevor\'s Office", 97.2707f, -1290.9940f, 29.2688f)',
    ("Yacht Example", -2045.18, -1031.33, -11.88, ["hei_yacht_heist_sv", "hei_yacht_heist_Lounge_sv"])
]

# Convert the Menyoo data
converted_data = convert_menyoo_to_my_format(menyoo_data)

# Print the converted data in your C++ format
print("const std::vector<TeleportInfo> vSafeHousesMenyoo =")
print("{")
for item in converted_data:
    x = item["coordinates"]["x"]
    y = item["coordinates"]["y"]
    z = item["coordinates"]["z"]
    heading = item["heading"]
    ipls_str = ", ".join([f'"{ipl}"_sv' for ipl in item["iplsToLoad"]])
    print(f'    {{\"{item["name"]}\", {{{x}f, {y}f, {z}f}}, {heading}f, {{{ipls_str}}}}},')
print("};")