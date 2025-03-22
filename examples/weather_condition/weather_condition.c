#include "weather_condition.h"

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