#ifndef WEATHER_CONDITION_H
#define WEATHER_CONDITION_H

#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sections.h"

typedef struct {
  char cities[100];
  double temperature;
  double wind_speed;
  int cloudiness;
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

typedef struct {
  void (*fetch_weather_data)(const char *url, WeatherData *data);
  char *(*build_url)(const char *base_url, double lat, double lon,
                     const char *units, const char *api_key);
} WeatherProvider;

char *build_url(const char *base_url, double lat_d, double lon_d,
                const char *units, const char *api_key);
void fetch_weather_data(const char *url, WeatherData *weather_data);
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  void *userp);
int download_temperature_data_sections(GeoLoc geoArray[],
                                       WeatherData weatherArray[],
                                       UrlData url_data, int start_idx,
                                       int end_idx);

int download_temperature_data_sections_mockable(GeoLoc geoArray[],
                                                WeatherData weatherArray[],
                                                UrlData url_data, int start_idx,
                                                int end_idx,
                                                WeatherProvider *provider);
#endif  // WEATHER_CONDITION_H