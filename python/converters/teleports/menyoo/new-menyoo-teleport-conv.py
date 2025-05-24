import re
import os

# Convert teleport locations from Menyoo format to my new format:

# Convert from:
# 			TeleLocation(true, 260.5322f, -999.1339f, -99.0087f, "Low End Apartment"),
# 			TeleLocation(true, 343.8500f, -999.0800f, -99.1977f, "Mid Range Apartment"),
# 			TeleLocation(true, -262.46f, -951.89f, 75.83f, "3 Alta Street Apt 10"),
# 			TeleLocation(true, -280.74f, -961.50f, 91.11f, "3 Alta Street Apt 57"),

# To this:
# {"Low End Apartment", Vector3(260.5322f, -999.1339f, -99.0087f)},
# {"Mid Range Apartment", Vector3(343.85f, -999.08f, -99.1977f)},
# {"3 Alta Street Apt 10", Vector3(-262.46f, -951.89f, 75.83f)},
# {"3 Alta Street Apt 57", Vector3(-280.74f, -961.5f, 91.11f)},

def convert_telelocation_format_from_file(input_filepath, output_filepath):
    """
    Reads TeleLocation entries from an input file, converts them to a C++ compatible format,
    and writes the result to an output file.

    Args:
        input_filepath: The path to the input file containing TeleLocation entries.
        output_filepath: The path to the output file where the converted C++ format will be written.
    """
    try:
        with open(input_filepath, 'r') as infile:
            telelocation_string = infile.read()
    except FileNotFoundError:
        print(f"Error: Input file not found at '{input_filepath}'")
        return
    except Exception as e:
        print(f"Error reading input file: {e}")
        return

    lines = telelocation_string.strip().split('\n')
    converted_locations = []

    # Regex to capture the X, Y, Z coordinates and the name.
    tele_regex = re.compile(
        r'TeleLocation\(true,\s*'
        r'([-]?\d+\.?\d*f?),\s*' # X value (optional 'f')
        r'([-]?\d+\.?\d*f?),\s*' # Y value (optional 'f')
        r'([-]?\d+\.?\d*f?),\s*' # Z value (optional 'f')
        r'"(.*?)"'             # Name
        r'\),?'                # Matches "),", optional trailing comma
    )

    for line in lines:
        line = line.strip()
        if not line: # Skip empty lines
            continue

        match = tele_regex.search(line)
        if match:
            x_str, y_str, z_str, name = match.groups()

            # Remove 'f' suffix and convert to float
            x_val = float(x_str.replace('f', ''))
            y_val = float(y_str.replace('f', ''))
            z_val = float(z_str.replace('f', ''))

            # Format the output string.
            # Using 6 decimal places for floats to match typical GTA V precision,
            # and adding 'f' for C++ float literals.
            converted_locations.append(
                f'\t\t\t{{"{name}", Vector3({x_val:.6f}f, {y_val:.6f}f, {z_val:.6f}f)}},'
            )
        else:
            print(f"Warning: Could not parse line: '{line}' - Skipping.")

    try:
        with open(output_filepath, 'w') as outfile:
            outfile.write("\n".join(converted_locations))
        print(f"\nConversion complete! Output written to '{output_filepath}'")
    except Exception as e:
        print(f"Error writing to output file: {e}")

# --- Main execution block ---
if __name__ == "__main__":
    # Define your input and output file paths
    input_file = 'menyoo_locations.txt'
    output_file = 'menyoo_locations.h'

    # Sample input data to create the file if it doesn't exist
    sample_tele_locations_input = """
            TeleLocation(true, 260.5322f, -999.1339f, -99.0087f, "Low End Apartment"),
            TeleLocation(true, 343.8500f, -999.0800f, -99.1977f, "Mid Range Apartment"),
            TeleLocation(true, -262.46f, -951.89f, 75.83f, "3 Alta Street Apt 10"),
            TeleLocation(true, -280.74f, -961.50f, 91.11f, "3 Alta Street Apt 57"),
            TeleLocation(true, 123.456f, 789.012f, 34.567f, "My Custom Location with spaces"),
            TeleLocation(true, -1.0f, -2.0f, -3.0f, "Another Spot"),
            TeleLocation(true, 1.23f, 4.56f, 7.89f, "Test with minimal precision"),
    """

    # Create a dummy input file for testing if it doesn't exist
    if not os.path.exists(input_file):
        print(f"Creating a sample input file '{input_file}'...")
        with open(input_file, 'w') as f:
            f.write(sample_tele_locations_input.strip()) # .strip() removes leading/trailing blank lines from the sample string
        print(f"Sample data written to '{input_file}'. You can now add your own data.")

    # Call the conversion function
    convert_telelocation_format_from_file(input_file, output_file)