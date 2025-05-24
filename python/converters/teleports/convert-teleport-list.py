# Convert a list of teleport locations to my format in C++
# These locations are obtained from here:
# https://gtaforums.com/topic/792877-list-of-over-100-coordinates-more-comming/


def convert_gta_locations(locations_string):
    """
    Converts a string of GTA 5 locations into a C++ compatible format,
    correctly extracting the location names.

    Args:
        locations_string: A string containing multiple location entries,
                          each on a new line, in the format:
                          "Name X:x_val Y:y_val Z:z_val"

    Returns:
        A string formatted for C++ ScriptHookV mod use.
    """
    lines = locations_string.strip().split('\n')
    converted_locations = []

    for line in lines:
        parts = line.strip().split(' ')

        # Find where the coordinates start to determine the name's end
        name_end_index = 0
        for i, part in enumerate(parts):
            if ':' in part and (part.startswith('X:') or part.startswith('Y:') or part.startswith('Z:')):
                name_end_index = i
                break

        # Extract the name and coordinates
        location_name = " ".join(parts[:name_end_index])

        coords = {}
        for part in parts[name_end_index:]:
            if ':' in part:
                key, value = part.split(':')
                coords[key] = float(value)

        x_val = coords.get('X')
        y_val = coords.get('Y')
        z_val = coords.get('Z')

        if x_val is not None and y_val is not None and z_val is not None:
            # Format the output string with proper float literals
            converted_locations.append(
                f'\t\t\t{{\"{location_name}\", Vector3({x_val:.2f}f, {y_val:.2f}f, {z_val}f)}}'
            )
        else:
            print(f"Warning: Could not parse coordinates for line: {line}")

    return ",\n".join(converted_locations)


# Your input string
gta_locations_input = """
Very High Up X:-129.964 Y:8130.873 Z:6705.307
IAA Roof X:134.085 Y:-637.859 Z:262.851
FIB Roof X:150.126 Y:-754.591 Z:262.865
Maze Bank Roof X:-75.015 Y:-818.215 Z:326.176
"""

# Perform the conversion
output_c_plus_plus = convert_gta_locations(gta_locations_input)
print(output_c_plus_plus)