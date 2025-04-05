#include "../include/weather_thread.h"

void *thread_worker(void *arg) {
  ThreadArgs *args = (ThreadArgs *)arg;

  download_temperature_data_sections(args->geoArray, args->weatherArray,
                                     args->url_data, args->start_idx,
                                     args->end_idx);

  sem_post(&args->semaphore);

  return NULL;
}

void *send_worker(void *arg) {
  ThreadArgs *args = (ThreadArgs *)arg;

  sem_wait(&args->semaphore);

  printf("Sending data for idx: %d - %d\n", args->start_idx, args->end_idx);
  for (int i = args->start_idx; i < args->end_idx; i++) {
    printf("Sending data for idx: %d\n", i);
    send_weather_data(args->client, args->weatherArray[i], nodeIDs[i]);
  }

  sem_post(&args->semaphore);
  sem_destroy(&args->semaphore);
  return NULL;
}

void fetch_weather_data_multithreaded(GeoLoc *geoArray,
                                      WeatherData *weatherArray,
                                      UrlData url_data, int loaded,
                                      char *server_url) {
  UA_Client *client = create_and_start_opc_ua_client(server_url);
  if (client == NULL) {
    printf("Failed to start client\n");
    return;
  }
  sleep(2);
  printf("Client is ready.\n");

  pthread_t threads[THREAD_COUNT];
  pthread_t send_threads[THREAD_COUNT];
  int chunk_size = loaded / THREAD_COUNT;

  for (int i = 0; i < THREAD_COUNT; i++) {
    ThreadArgs *args = malloc(sizeof(ThreadArgs));
    args->client = client;
    args->geoArray = geoArray;
    args->weatherArray = weatherArray;
    args->url_data = url_data;
    args->loaded = loaded;
    args->start_idx = i * chunk_size;
    args->end_idx = (i == THREAD_COUNT - 1) ? loaded : (i + 1) * chunk_size;

    sem_init(&args->semaphore, 0, 0);

    if (pthread_create(&threads[i], NULL, thread_worker, args) != 0) {
      perror("Error in creating fetching data thread");
      sem_destroy(&args->semaphore);
      free(args);
    }

    if (pthread_create(&send_threads[i], NULL, send_worker, args) != 0) {
      perror("Error in creating sending data thread");
      sem_destroy(&args->semaphore);
      free(args);
    }
  }

  for (int i = 0; i < THREAD_COUNT; i++) {
    pthread_join(threads[i], NULL);
    pthread_join(send_threads[i], NULL);
  }
  delete_opc_ua_client(client);
}