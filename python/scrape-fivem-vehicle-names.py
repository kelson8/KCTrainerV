import requests
from bs4 import BeautifulSoup
import json
import re


def scrape_fivem_vehicle_names(url="https://docs.fivem.net/docs/game-references/vehicle-references/vehicle-models/"):
    """
    Scrapes vehicle model names and their display names from the FiveM documentation.
    Adapted for the div-based structure as of May 2025.
    Returns a dictionary mapping lowercase model_name to display_name.
    """
    print(f"Scraping vehicle names from: {url}")

    try:
        response = requests.get(url, timeout=15)
        response.raise_for_status()  # Raise an exception for HTTP errors (4xx or 5xx)
    except requests.exceptions.RequestException as e:
        print(f"Error fetching URL: {e}")
        return None

    soup = BeautifulSoup(response.text, 'html.parser')

    vehicle_names_map = {}

    # Find all 'div' elements with the class 'vehicle'
    vehicle_divs = soup.find_all('div', class_='vehicle')

    if not vehicle_divs:
        print("No <div class='vehicle'> elements found. HTML structure might have changed again.")
        # Optionally, print a snippet of the page HTML to debug
        # print(soup.prettify()[:1000])
        return vehicle_names_map

    print(f"Found {len(vehicle_divs)} vehicle entries.")

    for vehicle_div in vehicle_divs:
        vehicle_info_div = vehicle_div.find('div', class_='vehicle-info')
        if not vehicle_info_div:
            # print("Warning: <div class='vehicle-info'> not found within a vehicle entry. Skipping.")
            continue

        model_name = ""
        display_name = ""

        # Find all <span> tags within vehicle-info
        spans = vehicle_info_div.find_all('span')
        for span in spans:
            strong_tag = span.find('strong')
            if strong_tag:
                label = strong_tag.text.strip()
                text_content = span.get_text(separator=' ', strip=True)  # Get all text, separate child elements

                # Remove the strong tag content from the text_content to get just the value
                value = text_content.replace(label, '').strip()

                if label == 'Model Name:':
                    model_name = value
                elif label == 'Display Name:':
                    display_name = value

        if model_name and display_name:
            vehicle_names_map[model_name.lower()] = display_name
        # else:
        # print(f"Warning: Could not extract Model Name or Display Name from a vehicle entry.")
        # print(vehicle_div.prettify()) # Print the problematic div for debugging

    print(f"Scraped {len(vehicle_names_map)} total vehicle names.")
    return vehicle_names_map


def main():
    vehicle_display_names = scrape_fivem_vehicle_names()
    if vehicle_display_names:
        output_json_path = 'fivem_vehicle_display_names.json'
        with open(output_json_path, 'w') as f:
            json.dump(vehicle_display_names, f, indent=4)
        print(f"Scraped vehicle display names saved to {output_json_path}")
    else:
        print("Failed to scrape vehicle names.")


if __name__ == "__main__":
    main()