#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/env_loader.h"
#include "../include/log.h"
#include "../include/opc_ua_server.h"

#define SECTIONS_FILE_PATH "resources/sections.csv"

int main() {
  log_set_level(LOG_DEBUG);

  FILE *logfile = fopen("logs/server_opc_ua.log", "a");
  if (logfile) {
    log_add_fp(logfile, LOG_DEBUG);
  }

  load_env(".env");
  const char *server_url = getenv("SERVER_URL");
  GeoLoc geoArray[MAX_RECORDS];
  allocate_geo_array(geoArray, MAX_RECORDS);

  int loaded =
      load_geo_data_from_csv(SECTIONS_FILE_PATH, geoArray, MAX_RECORDS);
  if (loaded > 0) {
    log_info("Loaded %d records.\n", loaded);
  } else {
    log_error("Failed to load records from file.\n");
  }
  create_and_start_opc_ua_server(server_url, geoArray);

  if (logfile) {
    fclose(logfile);
  }

  return 0;
}