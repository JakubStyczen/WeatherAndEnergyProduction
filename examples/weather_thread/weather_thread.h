#ifndef WEATHER_THREAD_H
#define WEATHER_THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../weather_condition/weather_condition.h"

#define THREAD_COUNT 10 

typedef struct {
    GeoLoc *geoArray;
    WeatherData *weatherArray;
    UrlData url_data;
    int start_idx;
    int end_idx;
    int loaded;
} ThreadArgs;

void *thread_worker(void *arg);
void fetch_weather_data_multithreaded(GeoLoc *geoArray, WeatherData *weatherArray, UrlData url_data, int loaded);

#endif // WEATHER_THREAD_H