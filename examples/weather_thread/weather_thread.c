#include "weather_thread.h"

void *thread_worker(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;

    download_temperature_data_sections(
        args->geoArray,
        args->weatherArray,
        args->url_data,
        args->start_idx,
        args->end_idx
    );

    free(arg);
    return NULL;
}

void fetch_weather_data_multithreaded(GeoLoc *geoArray, WeatherData *weatherArray, UrlData url_data, int loaded) {
    pthread_t threads[THREAD_COUNT];
    int chunk_size = loaded / THREAD_COUNT;

    for (int i = 0; i < THREAD_COUNT; i++) {
        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        args->geoArray = geoArray;
        args->weatherArray = weatherArray;
        args->url_data = url_data;
        args->loaded = loaded;
        args->start_idx = i * chunk_size;
        args->end_idx = (i == THREAD_COUNT - 1) ? loaded : (i + 1) * chunk_size;

        if (pthread_create(&threads[i], NULL, thread_worker, args) != 0) {
            perror("Error in creating thread");
            free(args);
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
}