#include "../include/opc_ua_client.h"

#include "../include/log.h"

void browse_node(UA_Client *client, int node_id) {
  UA_NodeId startNode = UA_NODEID_NUMERIC(1, node_id);
  printf("Browsing nodes under NodeId %d\n", startNode.identifier.numeric);

  UA_BrowseRequest bReq;
  UA_BrowseRequest_init(&bReq);
  bReq.requestedMaxReferencesPerNode = 0;
  bReq.nodesToBrowse = UA_BrowseDescription_new();
  bReq.nodesToBrowseSize = 1;

  bReq.nodesToBrowse[0].nodeId = startNode;
  bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;

  UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);

  if (bResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {
    for (size_t i = 0; i < bResp.resultsSize; i++) {
      for (size_t j = 0; j < bResp.results[i].referencesSize; j++) {
        UA_ReferenceDescription *ref = &bResp.results[i].references[j];

        log_info("Found Node: %.*s (NodeId: %d)\n",
                 (int)ref->browseName.name.length, ref->browseName.name.data,
                 ref->nodeId.nodeId.identifier.numeric);
      }
    }
  } else {
    log_error("Browse failed with status code: %x\n",
              bResp.responseHeader.serviceResult);
  }

  UA_BrowseResponse_clear(&bResp);
  UA_BrowseRequest_clear(&bReq);
}

void read_value_string(UA_Client *client, int node_id) {
  UA_Variant value;
  UA_Variant_init(&value);
  UA_StatusCode status = UA_Client_readValueAttribute(
      client, UA_NODEID_NUMERIC(1, node_id), &value);
  if (status == UA_STATUSCODE_GOOD &&
      UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_STRING])) {
    UA_String *str = (UA_String *)value.data;
    log_info("Value: %*s\n", (int)str->length, str->data);
  }
  UA_Variant_clear(&value);
}

void read_value_int32(UA_Client *client, int node_id) {
  UA_Variant value;
  UA_Variant_init(&value);
  UA_StatusCode status = UA_Client_readValueAttribute(
      client, UA_NODEID_NUMERIC(1, node_id), &value);
  if (status == UA_STATUSCODE_GOOD &&
      UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_INT32])) {
    printf("Value: %d\n", *(int *)value.data);
  }
  UA_Variant_clear(&value);
}

void read_value_double(UA_Client *client, int node_id) {
  UA_Variant value;
  UA_Variant_init(&value);
  UA_StatusCode status = UA_Client_readValueAttribute(
      client, UA_NODEID_NUMERIC(1, node_id), &value);
  if (status == UA_STATUSCODE_GOOD &&
      UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_DOUBLE])) {
    log_info("Value: %f\n", *(double *)value.data);
  }
  UA_Variant_clear(&value);
}

void write_to_string_node(UA_Client *client, int node_id, char *text) {
  UA_Variant value;
  UA_Variant_init(&value);
  UA_String uaString = UA_STRING_ALLOC(text);
  UA_Variant_setScalar(&value, &uaString, &UA_TYPES[UA_TYPES_STRING]);

  UA_StatusCode status = UA_Client_writeValueAttribute(
      client, UA_NODEID_NUMERIC(1, node_id), &value);

  if (status != UA_STATUSCODE_GOOD) {
    log_error("Error while saving string: %s\n", UA_StatusCode_name(status));
  }

  UA_String_clear(&uaString);
}

void write_to_double_node(UA_Client *client, int node_id, double new_value) {
  UA_Variant value;
  UA_Variant_init(&value);

  UA_Variant_setScalar(&value, &new_value, &UA_TYPES[UA_TYPES_DOUBLE]);

  UA_StatusCode status = UA_Client_writeValueAttribute(
      client, UA_NODEID_NUMERIC(1, node_id), &value);

  if (status != UA_STATUSCODE_GOOD) {
    log_error("Error while saving double: %s\n", UA_StatusCode_name(status));
  }
}

void write_to_int32_node(UA_Client *client, int node_id, int new_value) {
  UA_Variant value;
  UA_Variant_init(&value);

  UA_Variant_setScalar(&value, &new_value, &UA_TYPES[UA_TYPES_INT32]);

  UA_StatusCode status = UA_Client_writeValueAttribute(
      client, UA_NODEID_NUMERIC(1, node_id), &value);

  if (status != UA_STATUSCODE_GOOD) {
    log_error("Error while saving int32: %s\n", UA_StatusCode_name(status));
  }
}

void data_change_callback(UA_Client *client, UA_UInt32 subId, void *subContext,
                          UA_UInt32 monId, void *monContext,
                          UA_DataValue *value) {
  if (UA_Variant_hasScalarType(&value->value, &UA_TYPES[UA_TYPES_STRING])) {
    UA_String *str = (UA_String *)value->value.data;
    log_info("Received update: %.*s\n", (int)str->length, str->data);
  }
}

void add_subscription(UA_Client *client, int node_id) {
  UA_NodeId nodeId = UA_NODEID_NUMERIC(1, node_id);
  UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
  UA_CreateSubscriptionResponse response =
      UA_Client_Subscriptions_create(client, request, NULL, NULL, NULL);

  if (response.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {
    UA_MonitoredItemCreateRequest monRequest =
        UA_MonitoredItemCreateRequest_default(nodeId);
    UA_Client_MonitoredItems_createDataChange(
        client, response.subscriptionId, UA_TIMESTAMPSTORETURN_BOTH, monRequest,
        NULL, data_change_callback, NULL);
  }
}

void get_formatted_date(char *buffer, size_t bufferSize) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(buffer, bufferSize, "%d/%m/%Y %H:%M", t);
}

void send_weather_data(UA_Client *client, WeatherData weatherData,
                       int node_id) {
  char dateBuffer[17];
  get_formatted_date(dateBuffer, sizeof(dateBuffer));
  write_to_string_node(client, ++node_id, dateBuffer);
  write_to_double_node(client, ++node_id, weatherData.temperature);
  write_to_double_node(client, ++node_id, weatherData.wind_speed);
  write_to_int32_node(client, ++node_id, weatherData.cloudiness);
}

void delete_opc_ua_client(UA_Client *client) { UA_Client_delete(client); }

UA_Client *create_and_start_opc_ua_client(char *server_url) {
  UA_Client *client = UA_Client_new();
  UA_ClientConfig_setDefault(UA_Client_getConfig(client));

  UA_StatusCode status = UA_Client_connect(client, server_url);
  if (status != UA_STATUSCODE_GOOD) {
    UA_Client_delete(client);
    log_error("Failed to connect to server\n");
    return NULL;
  }
  log_info("Client connected to server: %s\n", server_url);
  return client;
}
