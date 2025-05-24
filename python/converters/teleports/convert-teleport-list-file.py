import os
import re

# Convert a list of teleport locations to my format in C++ from an input file.
# These locations are obtained from here:
# https://gtaforums.com/topic/792877-list-of-over-100-coordinates-more-comming/

# To use this, just convert the file, then in the header you will need to surround the locations with this:

# Rename vTeleportCategory to whatever category you are adding.
# This is the final format this script doesn't add, but it works well.
# 			const std::vector<TeleportInfo> vTeleportCategory =
#			{
# }

def convert_gta_locations_from_file(input_filepath, output_filepath):
    """
    Reads GTA 5 locations from an input file, converts them to a C++ compatible format,
    and writes the result to an output file. Handles empty lines and varied spacing in coordinates.

    Args:
        input_filepath: The path to the input file containing location entries.
        output_filepath: The path to the output file where the converted C++ format will be written.
    """
    try:
        with open(input_filepath, 'r') as infile:
            locations_string = infile.read()
    except FileNotFoundError:
        print(f"Error: Input file not found at '{input_filepath}'")
        return
    except Exception as e:
        print(f"Error reading input file: {e}")
        return

    lines = locations_string.strip().split('\n')
    converted_locations = []

    # Regex to find X, Y, Z coordinates.
    # It looks for X:, Y:, or Z:, followed by optional spaces, then a number (float or int).
    coord_regex = re.compile(r'([XYZ]):\s*([-]?\d+\.?\d*)')

    for line in lines:
        if not line.strip():  # Skip empty lines
            continue

        # Use regex to find all coordinates in the line
        matches = coord_regex.findall(line)

        coords = {}
        # Populate coords dictionary from regex matches
        for key, value_str in matches:
            try:
                coords[key] = float(value_str)
            except ValueError:
                print(f"Warning: Could not convert '{value_str}' to float for key '{key}' in line: {line}")
                coords = {}  # Invalidate this line's coordinates
                break  # Stop processing coordinates for this line

        x_val = coords.get('X')
        y_val = coords.get('Y')
        z_val = coords.get('Z')

        if x_val is not None and y_val is not None and z_val is not None:
            # Extract the location name by finding where the coordinate part starts
            # We'll split the line at the first occurrence of " X:" or " Y:" or " Z:"
            # to robustly get the name.
            name_match = re.match(r'^(.*?)\s*[XYZ]:', line)
            location_name = ""
            if name_match:
                location_name = name_match.group(1).strip()

            converted_locations.append(
                f'\t\t\t{{\"{location_name}\", Vector3({x_val:.2f}f, {y_val:.2f}f, {z_val}f)}}'
            )
        else:
            print(f"Warning: Skipping line due to missing or invalid coordinates: {line}")

    try:
        with open(output_filepath, 'w') as outfile:
            outfile.write(",\n".join(converted_locations))
        print(f"\nConversion complete! Output written to '{output_filepath}'")
    except Exception as e:
        print(f"Error writing to output file: {e}")


# --- Configuration ---
# Define your input and output file paths
input_file = 'gta_locations.txt'  # Make sure this file exists in the same directory as the script
output_file = 'converted_gta_locations.h'  # A common extension for header files

# --- Main execution ---
if __name__ == "__main__":
    # Create a dummy input file for testing if it doesn't exist
    if not os.path.exists(input_file):
        print(f"Creating a sample input file '{input_file}'...")
        with open(input_file, 'w') as f:
            f.write("Very High Up X:-129.964 Y:8130.873 Z:6705.307\n")
            f.write("IAA Roof X:134.085 Y:-637.859 Z:262.851\n")
            f.write("FIB Roof X:150.126 Y:-754.591 Z:262.865\n")
            f.write("Maze Bank Roof X:-75.015 Y:-818.215 Z:326.176\n")
            f.write("Another Location with Spaces X:100.0 Y:200.0 Z:300.0\n")
        print(f"Sample data written to '{input_file}'. You can now add your own data.")

    convert_gta_locations_from_file(input_file, output_file)