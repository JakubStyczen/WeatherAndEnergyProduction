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

char* build_url(const char* base_url, double lat_d, double lon_d, const char* units, const char* api_key) {
    // Assume URL format:
    // "https://api.openweathermap.org/data/2.5/weather?lat={lat}&lon={lon}&units={units}&appid={api_key}"
    char lat[11]; 
    char lon[11];
    sprintf(lat, "%f", lat_d); 
    sprintf(lon, "%f", lon_d); 
    
    size_t len = strlen(base_url) + strlen("lat=") + strlen(lat) + strlen("&lon=") + strlen(lon) + 
                    strlen("&units=") + strlen(units) + strlen("&appid=") + strlen(api_key) + 1;
    char* url = (char*)malloc(len);

    if (url == NULL) {
        printf("Error during URL memory allocation!\n");
        return NULL;
    }

    snprintf(url, len, "%slat=%s&lon=%s&units=%s&appid=%s", base_url, lat, lon, units, api_key);
    return url;
}

void fetch_weather_data(const char *url, WeatherData *weather_data) {
    double temperature_value = 0.0;
    double wind_speed_value = 0.0;
    int cloudiness_value = 0;
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1); 
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() has failed: %s\n", curl_easy_strerror(res));
        } else {
            cJSON *json = cJSON_Parse(chunk.memory);
            if (json == NULL) {
                printf("Error while parsing JSON\n");
                return;
            }

            cJSON *main = cJSON_GetObjectItem(json, "main");
            cJSON *temp = cJSON_GetObjectItem(main, "temp");
            cJSON *wind = cJSON_GetObjectItem(json, "wind");
            cJSON *wind_speed = cJSON_GetObjectItem(wind, "speed");
            cJSON *clouds = cJSON_GetObjectItem(json, "clouds");
            cJSON *cloudiness = cJSON_GetObjectItem(clouds, "all");

            if (temp != NULL && wind_speed != NULL && cloudiness != NULL) {
                // printf("Temperature: %.2f°C\n", temp->valuedouble);
                // printf("Wind speed: %.2f\n", wind_speed->valuedouble);
                // printf("Cloudiness: %d\n", cloudiness->valueint);
                weather_data->temperature = temp->valuedouble;
                weather_data->wind_speed = wind_speed->valuedouble;
                weather_data->cloudiness = cloudiness->valueint;
            }

            cJSON_Delete(json);
        }

        free(chunk.memory);

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

}

int download_temperature_data_sections(GeoLoc geoArray[], WeatherData weatherArray[], UrlData url_data, int start_idx, int end_idx){
    for (int i = start_idx; i < end_idx; i++) {
        printf("Miasto %d: %s, Lat: %.4f, Lon: %.4f\n", i, geoArray[i].cities, geoArray[i].lat, geoArray[i].lon);
        

        char *url = build_url(url_data.base_url, geoArray[i].lat, geoArray[i].lon, url_data.units, url_data.api_key);
        if (url == NULL) {
            return 1;
        }
        fetch_weather_data(url, &(weatherArray[i]));
            
        strncpy(weatherArray[i].cities, geoArray[i].cities, sizeof(weatherArray[i].cities) - 1);
        weatherArray[i].cities[sizeof(weatherArray[i].cities) - 1] = '\0';
        free(url);
    }   
}
