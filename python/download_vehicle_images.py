import requests
from bs4 import BeautifulSoup
import os
import time
from urllib.parse import urljoin  # <--- CRITICAL CHANGE HERE

import re


def download_vehicle_images(url="https://docs.fivem.net/docs/game-references/vehicle-references/vehicle-models/",
                            output_dir="vehicle_images"):
    """
    Scrapes vehicle image URLs from the FiveM documentation and downloads them.
    Images are saved to the specified output_dir.
    """
    print(f"Scraping image URLs from: {url}")

    try:
        response = requests.get(url, timeout=15)
        response.raise_for_status()  # Raise an exception for HTTP errors (4xx or 5xx)
    except requests.exceptions.RequestException as e:
        print(f"Error fetching URL: {e}")
        return

    soup = BeautifulSoup(response.text, 'html.parser')

    # Create the output directory if it doesn't exist
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
        print(f"Created output directory: {output_dir}")

    # Find all 'div' elements with the class 'vehicle'
    vehicle_divs = soup.find_all('div', class_='vehicle')

    if not vehicle_divs:
        print("No <div class='vehicle'> elements found. HTML structure might have changed.")
        return

    print(f"Found {len(vehicle_divs)} vehicle entries for image download.")

    download_count = 0
    skipped_count = 0
    error_count = 0

    # Ensure base_url has a trailing slash for correct relative joining
    # urljoin needs a base URL that ends with a slash if it's a directory,
    # or it will strip the last component if it's treated as a file.
    base_url = url if url.endswith('/') else url + '/'

    for i, vehicle_div in enumerate(vehicle_divs):
        img_tag = vehicle_div.find('img')
        if not img_tag:
            continue

        img_src = img_tag.get('src')
        img_alt = img_tag.get('alt')

        if not img_src:
            print(f"Warning: <img> tag in vehicle entry {i + 1} has no 'src' attribute. Skipping.")
            continue

        # Construct the full image URL using urllib.parse.urljoin
        full_img_url = urljoin(base_url, img_src)  # <--- CORRECTED USAGE HERE

        # Determine filename
        if img_alt:
            filename_base = re.sub(r'[^a-zA-Z0-9_]', '_', img_alt.lower()).strip('_')
            filename = f"{filename_base}.webp"
        else:
            filename = os.path.basename(img_src)

        output_path = os.path.join(output_dir, filename)

        if os.path.exists(output_path):
            skipped_count += 1
            continue

        try:
            print(f"Downloading {full_img_url} to {output_path}...")
            img_response = requests.get(full_img_url, timeout=10)
            img_response.raise_for_status()

            with open(output_path, 'wb') as f:
                f.write(img_response.content)
            download_count += 1
            # Optional: time.sleep(0.1)

        except requests.exceptions.RequestException as e:
            print(f"ERROR: Failed to download {full_img_url}: {e}")
            error_count += 1
        except IOError as e:
            print(f"ERROR: Failed to write file {output_path}: {e}")
            error_count += 1

    print("\n--- Download Summary ---")
    print(f"Total downloaded: {download_count}")
    print(f"Total skipped (already exists): {skipped_count}")
    print(f"Total errors: {error_count}")
    print("------------------------")


if __name__ == "__main__":
    download_vehicle_images()