#include "../include/opc_ua_server.h"

#include "../include/log.h"

static void add_object(UA_Server *server, char *name, int nodeID,
                       UA_NodeId parent_nodeID) {
  UA_NodeId myObject;
  UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
  oAttr.displayName = UA_LOCALIZEDTEXT("en-US", name);

  UA_Server_addObjectNode(
      server, UA_NODEID_NUMERIC(1, nodeID), parent_nodeID,
      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, name),
      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), oAttr, NULL, &myObject);
}

void add_string_node(UA_Server *server, char *name, int nodeID,
                     int parent_nodeID) {
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.displayName = UA_LOCALIZEDTEXT("en-US", name);
  attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

  UA_String initialString = UA_STRING("01/01/2000 00:00");
  UA_Variant_setScalar(&attr.value, &initialString, &UA_TYPES[UA_TYPES_STRING]);

  UA_Server_addVariableNode(
      server, UA_NODEID_NUMERIC(1, nodeID), UA_NODEID_NUMERIC(1, parent_nodeID),
      UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), UA_QUALIFIEDNAME(1, name),
      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), attr, NULL, NULL);
}

void add_double_node(UA_Server *server, char *name, int nodeID,
                     int parent_nodeID) {
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.displayName = UA_LOCALIZEDTEXT("en-US", name);
  attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

  double initialValue = 0.0;

  UA_Variant_setScalar(&attr.value, &initialValue, &UA_TYPES[UA_TYPES_DOUBLE]);

  UA_Server_addVariableNode(
      server, UA_NODEID_NUMERIC(1, nodeID), UA_NODEID_NUMERIC(1, parent_nodeID),
      UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), UA_QUALIFIEDNAME(1, name),
      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), attr, NULL, NULL);
}

void add_int32_node(UA_Server *server, char *name, int nodeID,
                    int parent_nodeID) {
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.displayName = UA_LOCALIZEDTEXT("en-US", name);
  attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

  int initialValue = 0;

  UA_Variant_setScalar(&attr.value, &initialValue, &UA_TYPES[UA_TYPES_INT32]);

  UA_NodeId nodeId = UA_NODEID_NUMERIC(1, nodeID);
  UA_NodeId parentNodeId = UA_NODEID_NUMERIC(1, parent_nodeID);
  UA_Server_addVariableNode(
      server, nodeId, parentNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
      UA_QUALIFIEDNAME(1, name),
      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), attr, NULL, NULL);
}

void add_weather_object(UA_Server *server, char *name, int nodeID,
                        int parent_nodeID) {
  add_object(server, name, nodeID, UA_NODEID_NUMERIC(1, parent_nodeID));

  add_string_node(server, "Date", nodeID + 1, nodeID);
  add_double_node(server, "Temperature", nodeID + 2, nodeID);
  add_double_node(server, "WindSpeed", nodeID + 3, nodeID);
  add_int32_node(server, "Cloudiness", nodeID + 4, nodeID);
}

void add_weather_object_for_every_section(UA_Server *server, GeoLoc array[],
                                          int parent_nodeID) {
  int baseID = 2101;
  int offset = 100;
  int j = 0;
  for (int i = 0; i < DATA_ARRAY_SIZE; i++) {
    char *locationName = array[i].cities;
    int nodeID = baseID;
    if (j != 0) {
      nodeID = nodeID + j * offset;
    }
    j++;
    if (j == 9) {
      j = 0;
      baseID = baseID + 10;
    }
    add_weather_object(server, locationName, nodeID, parent_nodeID);
  }
}

void add_average_weather_object(UA_Server *server, int parent_nodeID) {
  add_string_node(server, "Date", parent_nodeID + 1, parent_nodeID);
  add_double_node(server, "Temperature", parent_nodeID + 2, parent_nodeID);
  add_double_node(server, "WindSpeed", parent_nodeID + 3, parent_nodeID);
  add_int32_node(server, "Cloudiness", parent_nodeID + 4, parent_nodeID);
}

void add_energy_data_objects(UA_Server *server, int parent_nodeID) {
  add_double_node(server, "Thermal", parent_nodeID + 1, parent_nodeID);
  add_double_node(server, "Water", parent_nodeID + 2, parent_nodeID);
  add_double_node(server, "Wind", parent_nodeID + 3, parent_nodeID);
  add_double_node(server, "Solar", parent_nodeID + 4, parent_nodeID);
  add_double_node(server, "Other", parent_nodeID + 5, parent_nodeID);
  add_double_node(server, "CZ", parent_nodeID + 6, parent_nodeID);
  add_double_node(server, "DE", parent_nodeID + 7, parent_nodeID);
  add_double_node(server, "SK", parent_nodeID + 8, parent_nodeID);
  add_double_node(server, "LT", parent_nodeID + 9, parent_nodeID);
  add_double_node(server, "UA", parent_nodeID + 10, parent_nodeID);
  add_double_node(server, "SE", parent_nodeID + 11, parent_nodeID);
  add_double_node(server, "Frequency", parent_nodeID + 12, parent_nodeID);
  add_string_node(server, "Date", parent_nodeID + 13, parent_nodeID);
}

void create_and_start_opc_ua_server(char *server_url, GeoLoc array[]) {
  UA_Server *server = UA_Server_new();
  UA_ServerConfig *config = UA_Server_getConfig(server);

  UA_String customServerUrls[1];
  customServerUrls[0] = UA_STRING(server_url);
  size_t customServerUrlsSize = 1;

  config->serverUrls = customServerUrls;
  config->serverUrlsSize = customServerUrlsSize;
  config->applicationDescription.applicationName =
      UA_LOCALIZEDTEXT_ALLOC("en", "Example for Medium");

  UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
  add_object(server, "Weather station", 1001, parentNodeId);
  add_object(server, "Weather data", 2001, UA_NODEID_NUMERIC(1, 1001));
  add_object(server, "Average data", 3001, UA_NODEID_NUMERIC(1, 1001));
  add_object(server, "Energy data", 4001, UA_NODEID_NUMERIC(1, 1001));

  add_weather_object_for_every_section(server, array, 2001);

  add_average_weather_object(server, 3001);

  add_energy_data_objects(server, 4001);

  log_info("OPC UA Server started...\n");
  UA_Server_runUntilInterrupt(server);
  UA_Server_delete(server);
}
