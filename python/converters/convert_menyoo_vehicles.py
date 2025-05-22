import re
import os

# Generate a generated_vehicle_info_list.txt from the menyoo_vehicles.txt
# I copied those vehicles from VehicleSpawner.cpp in Menyoo from this:
# std::vector<VehBmpSprite> vVehicleBmps{}

def generate_display_name(enum_name, texture_name):
    """
    Generates a user-friendly display name from enum_name or texture_name.
    Prioritizes a lookup table, then tries to format the texture name,
    then the enum name as a fallback.
    """
    # 1. Manual Lookup Table for specific, custom names
    # This dictionary maps the ENUM_NAME (without 'VEHICLE_') or texture_name
    # to the desired display name.
    # IMPORTANT: Update this with your preferred names!
    manual_names = {
        "JESTER2": "Jester Classic",
        "ELEGY2": "Elegy Retro Custom",
        "YOUGA5": "Youga Classic 4x4",
        "TACO": "Taco Truck",
        "SCRAMJET": "Scramjet",  # Add any others you want specific names for
        "T20": "T20",
        "ZENTORNO": "Zentorno",
    }

    # Clean enum name (e.g., "VEHICLE_POLCOQUETTE4" -> "POLCOQUETTE4")
    clean_enum = enum_name.replace("VEHICLE_", "").upper()
    if clean_enum in manual_names:
        return manual_names[clean_enum]

    # Try formatting texture_name if available and reasonable
    # This is a fallback if no manual_name is found.
    if texture_name and texture_name != "vehModel":  # "vehModel" is often a generic placeholder
        # Simple capitalization and space insertion for camelCase/snake_case
        formatted_texture_name = re.sub(r'([a-z])([A-Z])', r'\1 \2', texture_name)  # CamelCase -> Space
        formatted_texture_name = formatted_texture_name.replace('_',
                                                                ' ').title()  # snake_case -> Spaces, then Title Case
        if formatted_texture_name:
            return formatted_texture_name

    # Fallback: Format the clean enum name
    formatted_enum_name = re.sub(r'([a-z])([A-Z])', r'\1 \2', clean_enum)  # CamelCase -> Space
    formatted_enum_name = formatted_enum_name.replace('_', ' ').title()  # snake_case -> Spaces, then Title Case
    return formatted_enum_name


def convert_menyoo_format(input_file_path, output_file_path):
    output_lines = []

    with open(input_file_path, 'r') as infile:
        for line_num, line in enumerate(infile, 1):
            line = line.strip()
            if not line or line.startswith('//'):  # Skip empty lines and comments
                continue

            # Regex to capture the enum, texture_dict (ignored), and texture_name
            # Example: { VEHICLE_POLCOQUETTE4, "candc_dlc_2024_2", "polcoqutt4" },
            # match = re.match(r'\{\s*(VEHICLE_[A-Z0-9_]+),\s*"([^"]*)",\s*"([^"]*)"\s*\},?', line)

            # REFINED REGEX:
            # - More explicit about potential leading/trailing whitespace around comma
            # - Relaxed matching around strings (though `[^"]*` is usually robust)
            # match = re.match(r'\{\s*(VEHICLE_[A-Z0-9_]+)\s*,\s*"([^"]*)",\s*"([^"]*)"\s*\},?', line)

            # New version
            # Refined Regex:
            # Changed '\s*,' to '\s*,\s*' to ensure robust matching around commas.
            # Also made the trailing whitespace at the end of the line more robust.
            match = re.match(r'\{\s*(VEHICLE_[A-Z0-9_]+)\s*,\s*"([^"]*)"\s*,\s*"([^"]*)"\s*\},?', line)

            if match:
                enum_name = match.group(1)
                # texture_dict = match.group(2) # We don't need this if VehicleInfo doesn't store it
                texture_name = match.group(3)

                display_name = generate_display_name(enum_name, texture_name)

                # Format for your VehicleInfo struct: { std::string name, VehicleHash hashEnum }
                # Note: The enum_name (e.g., VEHICLE_POLCOQUETTE4) is directly put in output
                output_line = f'    {{ "{display_name}", {enum_name} }},'
                output_lines.append(output_line)
            else:
                print(f"WARNING: Could not parse line {line_num}: {line}")
                output_lines.append(
                    f'// WARNING: Failed to parse line {line_num}: {line}')  # Keep unparsed lines for review

    with open(output_file_path, 'w') as outfile:
        for output_line in output_lines:
            outfile.write(output_line + '\n')

    print(f"Conversion complete. Output written to {output_file_path}")


if __name__ == "__main__":
    input_file = 'menyoo_vehicles.txt'
    output_file = 'generated_vehicle_info_list.txt'

    # Create a dummy input file for testing if it doesn't exist
    if not os.path.exists(input_file):
        print(f"'{input_file}' not found. Creating a dummy for demonstration.")
        with open(input_file, 'w') as f:
            f.write('{ VEHICLE_POLCOQUETTE4, "candc_dlc_2024_2", "polcoqutt4" },\n')
            f.write('{ VEHICLE_POLFACTION2, "candc_dlc_2024_2", "polfation2" },\n')
            f.write('{ VEHICLE_URANUS, "sssa_dlc_2024_2", "uranus" },\n')
            f.write('{ VEHICLE_TITAN2, "candc_dlc_2024_2", "titan2" },\n')
            f.write('{ VEHICLE_YOUGA5, "ytd", "vehModel" },\n')  # Example with different texture dict/name
            f.write('// This is a comment line\n')
            f.write('{ VEHICLE_JESTER2, "dlc_ind_4", "jester2" },\n')
            f.write('{ VEHICLE_ELEGY2, "dlc_ind_5", "elegy2" },\n')
            f.write('{ VEHICLE_TACO, "candc_dlc_2024_2", "taco" },\n')  # Example with custom name
            f.write('{ VEHICLE_SCRAMJET, "dlc_scramjet", "scramjet" },\n')  # Example for manual override

    convert_menyoo_format(input_file, output_file)