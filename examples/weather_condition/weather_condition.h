#ifndef WEATHER_CONDITION_H
#define WEATHER_CONDITION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "../geo_loc_sections/sections.h"

#define MAX_LOCATIONS 100

typedef struct {
    char city[100];
    double temperature;
} WeatherData;

struct MemoryStruct {
    char *memory;
    size_t size;
};


char* build_url(const char* base_url, const char* lat, const char* lon, const char* units, const char* api_key);
void get_temperature(const char* url);
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

#endif // WEATHER_CONDITION_H