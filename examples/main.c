#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "env_loader/env_loader.h"
#include "weather_thread/weather_thread.h"

#define SECTIONS_FILE_PATH "geo_loc_sections/sections.csv"

int main() {
    load_env(".env");

    const char *base_url = "https://api.openweathermap.org/data/2.5/weather?";
    const char *lat = "50";
    const char *lon = "20";
    const char *api_key = getenv("API_KEY");
    const char *units = getenv("units");

    UrlData url_data;
    url_data.base_url = base_url;
    url_data.units = units;
    url_data.api_key = api_key;

    GeoLoc geoArray[MAX_RECORDS];
    WeatherData weatherArray[MAX_RECORDS];
    allocate_geo_array(geoArray, MAX_RECORDS);

    int loaded = load_geo_data_from_csv(SECTIONS_FILE_PATH, geoArray, MAX_RECORDS);
    if (loaded > 0) {
        printf("Loaded %d records.\n", loaded);
    } else {
        printf("Failed to load records from file.\n");
    }

    // int status = download_temperature_data_sections(geoArray, weatherArray, url_data, 0, loaded);
    fetch_weather_data_multithreaded(geoArray, weatherArray, url_data, loaded);

    // if (status != -1) {
    printf("\nTemperature list:\n");
    for (int i = 0; i < loaded; i++) {
        printf("Cities: %s, Temperature: %.2f\n", weatherArray[i].cities, weatherArray[i].temperature);
    }
    return 0;
    // } else {
    //     printf("\nFailed to read all temperatues:\n");
    //     return -1;
    // }
}