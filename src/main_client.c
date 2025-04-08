#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/env_loader.h"
#include "../include/log.h"
#include "../include/opc_ua_client.h"
#include "../include/weather_thread.h"

#define SECTIONS_FILE_PATH "resources/sections.csv"

int main() {
  log_set_level(LOG_DEBUG);

  FILE *logfile = fopen("logs/client_opc_ua.log", "a");
  if (logfile) {
    log_add_fp(logfile, LOG_DEBUG);
  }

  load_env(".env");

  const char *base_url = "https://api.openweathermap.org/data/2.5/weather?";
  const char *api_key = getenv("API_KEY");
  const char *units = getenv("UNITS");
  const char *server_url = getenv("SERVER_URL");

  UrlData url_data;
  url_data.base_url = base_url;
  url_data.units = units;
  url_data.api_key = api_key;

  GeoLoc geoArray[MAX_RECORDS];
  WeatherData weatherArray[MAX_RECORDS];
  allocate_geo_array(geoArray, MAX_RECORDS);

  int loaded =
      load_geo_data_from_csv(SECTIONS_FILE_PATH, geoArray, MAX_RECORDS);
  if (loaded > 0) {
    log_info("Loaded %d records.\n", loaded);
  } else {
    log_error("Failed to load records from file.\n");
  }
  fetch_weather_data_multithreaded(geoArray, weatherArray, url_data, loaded,
                                   server_url);
  for (int i = 0; i < loaded; i++) {
    log_trace(
        "Cities: %s, Temperature: %.2f, Wind speed:%.2f, Cloudiness:%d%%\n",
        weatherArray[i].cities, weatherArray[i].temperature,
        weatherArray[i].wind_speed, weatherArray[i].cloudiness);
  }

  if (logfile) {
    fclose(logfile);
  }

  return 0;
}