#ifndef WEATHER_THREAD_H
#define WEATHER_THREAD_H

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include "../opc_ua/opc_ua_client/opc_ua_client.h"
#include "../weather_condition/weather_condition.h"

#define THREAD_COUNT 10

typedef struct {
  GeoLoc *geoArray;
  WeatherData *weatherArray;
  UrlData url_data;
  int start_idx;
  int end_idx;
  int loaded;
  sem_t semaphore;
  UA_Client *client;
} ThreadArgs;

void *thread_worker(void *arg);
void *send_worker(void *arg);
void fetch_weather_data_multithreaded(GeoLoc *geoArray,
                                      WeatherData *weatherArray,
                                      UrlData url_data, int loaded,
                                      char *server_url);

#endif  // WEATHER_THREAD_H