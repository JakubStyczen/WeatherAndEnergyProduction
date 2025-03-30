#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env_loader/env_loader.h"
#include "opc_ua/opc_ua_server/opc_ua_server.h"

#define SECTIONS_FILE_PATH "geo_loc_sections/sections.csv"

int main() {
  load_env(".env");
  const char *server_url = getenv("SERVER_URL");

  GeoLoc geoArray[MAX_RECORDS];
  allocate_geo_array(geoArray, MAX_RECORDS);

  int loaded =
      load_geo_data_from_csv(SECTIONS_FILE_PATH, geoArray, MAX_RECORDS);
  if (loaded > 0) {
    printf("Loaded %d records.\n", loaded);
  } else {
    printf("Failed to load records from file.\n");
  }
  create_and_start_opc_ua_server(server_url, geoArray);
  return 0;
}