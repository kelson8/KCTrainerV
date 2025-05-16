import re

# Convert a list of Menyoo locations into a python tuple, for use in convert_menyoo_locations_my_format.py

def convert_cpp_to_menyoo_format(cpp_code):
    """
    Converts a C++ std::vector of TeleLocation to a list of Menyoo-style strings.

    Args:
        cpp_code: A string containing the C++ code snippet.

    Returns:
        A list of strings representing Menyoo TeleLocation entries.
    """
    menyoo_list = []
    matches = re.findall(r'TeleLocation\("([^"]*)",\s*([-.\d]+)f,\s*([-.\d]+)f,\s*([-.\d]+)f(?:,\s*\{(.*?)\})?(?:,\s*\{(.*?)\})?\)', cpp_code)

    for match in matches:
        name = match[0].replace("'", "\\'")  # Escape single quotes in the name
        x = float(match[1])
        y = float(match[2])
        z = float(match[3])
        ipls_load_str = match[4]

        menyoo_entry = f'\'TeleLocation("{name}", {x:.6f}f, {y:.6f}f, {z:.6f}f'
        if ipls_load_str:
            ipls_load = [ipl.strip().strip('"') for ipl in ipls_load_str.split(',') if ipl.strip()]
            quoted_ipls = [f'"{ipl}_sv"' for ipl in ipls_load]
            menyoo_entry += f',{{ {", ".join(quoted_ipls)} }}'
            ipls_unload_str = match[5]
            if ipls_unload_str:
                ipls_unload = [ipl.strip().strip('"') for ipl in ipls_unload_str.split(',') if ipl.strip()]
                quoted_ipls_unload = [f'"{ipl}_sv"' for ipl in ipls_unload]
                menyoo_entry += f',{{ {", ".join(quoted_ipls_unload)} }}'
        # menyoo_entry += ')\''
        menyoo_entry += ')\','
        menyoo_list.append(menyoo_entry)

    return menyoo_list

# C++ code snippet
cpp_code = """
#pragma region safehouses
		 const std::vector<TeleLocation> vSafeHouses
		 {
			 TeleLocation("Michael's House", -813.603f, 179.474f, 72.1548f),
			 TeleLocation("Franklin's House (New)", 7.11903f, 536.615f, 176.028f,{ "v_franklinshouse"_sv, "unlocked"_sv },{ "locked"_sv}),
			 TeleLocation("Franklin's House (Old)", -14.3803f, -1438.51f, 31.1073f),
			 TeleLocation("Trevor's House", 1972.61f, 3817.04f, 33.4278f),
			 TeleLocation("Trevor's House 2", -1151.77f, -1518.14f, 10.6327f),
			 TeleLocation("Trevor's Office", 97.2707f, -1290.9940f, 29.2688f),
		 };
#pragma endregion
"""

# Convert the C++ data
menyoo_data = convert_cpp_to_menyoo_format(cpp_code)

# Print the Menyoo data in the desired Python list format
print("menyoo_data = [")
for entry in menyoo_data:
    print(f"    {entry}")
print("]")