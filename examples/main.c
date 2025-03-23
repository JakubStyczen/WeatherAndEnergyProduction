#include <stdio.h>
#include <stdlib.h>
#include "env_loader/env_loader.h"

#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

// Struktura do przechowywania odpowiedzi HTTP
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Funkcja do zapisywania danych z odpowiedzi GET w pamięci
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        printf("Błąd alokacji pamięci!\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

// Funkcja do wysyłania zapytania GET i parsowania odpowiedzi JSON
void get_temperature(const char *url) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  // Inicjalizacja pustej pamięci
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Ustawienie URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Ustawienie funkcji callback do zapisywania odpowiedzi
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // Wysłanie zapytania GET
        res = curl_easy_perform(curl);

        // Sprawdzenie, czy zapytanie zakończyło się powodzeniem
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() nie powiodło się: %s\n", curl_easy_strerror(res));
        } else {
            // Parsowanie odpowiedzi JSON
            cJSON *json = cJSON_Parse(chunk.memory);
            if (json == NULL) {
                printf("Błąd podczas parsowania JSON\n");
                return;
            }

            // Wyciąganie temperatury
            cJSON *main = cJSON_GetObjectItem(json, "main");
            cJSON *temp = cJSON_GetObjectItem(main, "temp");

            if (temp != NULL) {
                printf("Temperatura wynosi: %.2f K\n", temp->valuedouble);
                // Przeliczenie na Celsjusza
                double celsius = temp->valuedouble - 273.15;
                printf("Temperatura wynosi: %.2f°C\n", celsius);
            }

            // Zwolnienie pamięci
            cJSON_Delete(json);
        }

        // Zwolnienie pamięci odpowiedzi
        free(chunk.memory);

        // Zakończenie używania libcurl
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

// Funkcja do budowania URL z dynamicznymi zmiennymi (miasto i klucz API)
char* build_url(const char* base_url, const char* lat, const char* lon, const char* units, const char* api_key) {
    // Zakładając, że URL jest w formacie:
    // "https://api.openweathermap.org/data/2.5/weather?&appid={api_key}"
    size_t len = strlen(base_url) + strlen("lat=") + strlen(lat) + strlen("&lon=") + strlen(lon) + 
                    strlen("&units=") + strlen(units) + strlen("&appid=") + strlen(api_key) + 1;
    char* url = (char*)malloc(len);

    if (url == NULL) {
        printf("Błąd alokacji pamięci na URL!\n");
        return NULL;
    }

    snprintf(url, len, "%slat=%s&lon=%s&units=%s&appid=%s", base_url, lat, lon, units, api_key);
    // snprintf(url, len, "%slat=%s&lon=%s", base_url, lat, lon);
    return url;
}

int main() {
    load_env(".env");

    const char *base_url = "https://api.openweathermap.org/data/2.5/weather?";

    const char *lat = "50";
    const char *lon = "20";

    const char *api_key = getenv("API_KEY");
    const char *units = getenv("units");

    if (api_key) printf("API_KEY: %s", api_key);

    char *url = build_url(base_url, lat, lon, units, api_key);
    if (url == NULL) {
        return 1;  // Zakończenie programu, jeśli nie udało się alokować pamięci dla URL
    }

    printf("API_KEY: %s\n", url);

    // Wywołanie funkcji do pobrania i wyświetlenia temperatury
    get_temperature(url);

    // Zwolnienie pamięci dla URL
    free(url);

    return 0;
}