#ifndef WEATHER_CONDITION_H
#define WEATHER_CONDITION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "../geo_loc_sections/sections.h"

typedef struct {
    char cities[100];
    double temperature;
} WeatherData;

struct MemoryStruct {
    char *memory;
    size_t size;
};

typedef struct {
    const char *base_url;
    const char *api_key;
    const char *units;
} UrlData;

char* build_url(const char* base_url, double lat_d, double lon_d, const char* units, const char* api_key);
double get_temperature(const char* url);
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
int download_temperature_data_sections(GeoLoc geoArray[], WeatherData weatherArray[], UrlData url_data, int start_idx, int end_idx);

#endif // WEATHER_CONDITION_H