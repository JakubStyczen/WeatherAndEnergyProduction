#ifndef SECTIONS_H
#define SECTIONS_H

#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_RECORDS 100

typedef struct {
    char cities[100];  // cities names
    double lat;        // geo latitude
    double lon;        // geo longtitute
} GeoLoc;

void allocate_geo_array(GeoLoc array[], int size);

int add_geo_record(GeoLoc array[], int size, const char* cities, double lat, double lon);

int load_geo_data_from_csv(const char* filename, GeoLoc array[], int max_size);

#endif // SECTIONS_H