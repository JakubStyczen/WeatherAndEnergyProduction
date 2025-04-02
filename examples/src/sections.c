#include "../include/sections.h"

void allocate_geo_array(GeoLoc array[], int size) {
  for (int i = 0; i < size; i++) {
    array[i].cities[0] = '\0';
    array[i].lat = 0;
    array[i].lon = 0;
  }
}

int add_geo_record(GeoLoc array[], int size, const char* cities, double lat,
                   double lon) {
  for (int i = 0; i < size; i++) {
    if (array[i].cities[0] == '\0') {
      strncpy(array[i].cities, cities, sizeof(array[i].cities) - 1);
      array[i].cities[sizeof(array[i].cities) - 1] = '\0';
      array[i].lat = lat;
      array[i].lon = lon;
      return 0;
    }
  }
  printf("Space limit, no free space!\n");
  return -1;
}

int load_geo_data_from_csv(const char* filename, GeoLoc array[], int max_size) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    perror("Nie można otworzyć pliku");
    return -1;
  }

  char line[MAX_LINE_LENGTH];
  int count = 0;

  while (fgets(line, sizeof(line), file) && count < max_size) {
    char city[100];
    double lat, lon;

    if (sscanf(line, "%99[^,],%lf,%lf", city, &lat, &lon) == 3) {
      add_geo_record(array, max_size, city, lat, lon);
      count++;
    } else {
      printf("Błąd parsowania linii: %s", line);
    }
  }

  fclose(file);
  return count;
}