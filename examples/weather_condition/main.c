#include "weather_condition.h"

#define API_KEY ""

int main() {
    GeoLoc geoArray[MAX_RECORDS];  // Statyczna tablica GeoLoc
    allocate_geo_array(geoArray, MAX_RECORDS);

    // Wczytujemy dane z pliku CSV
    int loaded = load_geo_data_from_csv("../geo_loc_sections/sections.csv", geoArray, MAX_RECORDS);
    if (loaded > 0) {
        printf("Załadowano %d rekordów.\n", loaded);
    } else {
        printf("Nie udało się załadować danych.\n");
    }

    // Wyświetlamy wczytane dane
    printf("\nLista lokalizacji:\n");
    for (int i = 0; i < loaded; i++) {
        printf("Miasto: %s, Lat: %.4f, Lon: %.4f\n", geoArray[i].cities, geoArray[i].lat, geoArray[i].lon);
    }


    return 0;
}