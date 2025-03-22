#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/types_generated.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_VALUES 5  // Liczba przechowywanych wartości

static UA_Int32 valueBuffer[MAX_VALUES] = {0};
static int bufferIndex = 0;

// Callback obsługujący zapis nowej wartości
static UA_StatusCode writeVariableCallback(UA_Server *server,
                                           const UA_NodeId *sessionId, void *sessionContext,
                                           const UA_NodeId *nodeId, void *nodeContext,
                                           const UA_NumericRange *range, const UA_DataValue *data) {
    if (UA_Variant_hasScalarType(&data->value, &UA_TYPES[UA_TYPES_INT32])) {
        valueBuffer[bufferIndex] = *(UA_Int32 *)data->value.data;
        bufferIndex = (bufferIndex + 1) % MAX_VALUES;  // Rotacyjne zapisywanie wartości
        printf("Nowa wartość zapisana: %d\n", valueBuffer[(bufferIndex - 1 + MAX_VALUES) % MAX_VALUES]);
        return UA_STATUSCODE_GOOD;
    }
    return UA_STATUSCODE_BADTYPEMISMATCH;
}

int main() {
    UA_Server *server = UA_Server_new();
    UA_ServerConfig *config = UA_Server_getConfig(server);
    UA_String customServerUrls[1];
    customServerUrls[0] = UA_STRING("opc.tcp://192.168.192.26:4840");
    size_t customServerUrlsSize = 1;

    config->serverUrls = customServerUrls;
    config->serverUrlsSize = customServerUrlsSize;
    config->applicationDescription.applicationName = UA_LOCALIZEDTEXT_ALLOC("en", "Example for Medium");
    
    
    // Tworzenie atrybutów zmiennej
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "MyVariable");

    UA_Int32 newValue = 0;
    UA_Variant_setScalar(&attr.value, &newValue, &UA_TYPES[UA_TYPES_INT32]);

    // Dodanie zmiennej do serwera
    UA_NodeId myNodeId = UA_NODEID_STRING(1, "my.variable");
    UA_QualifiedName myName = UA_QUALIFIEDNAME(1, "MyVariable");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_Server_addVariableNode(server, myNodeId, parentNodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              myName, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                              attr, NULL, NULL);

    // Ustawienie callbacka na zapis
    UA_ValueCallback callback;
    callback.onWrite = writeVariableCallback;
    UA_Server_setVariableNode_valueCallback(server, myNodeId, callback);

    // Uruchomienie serwera
    // UA_Boolean running = true;
    // signal(SIGINT, [](int sign){ running = false; });
    // UA_StatusCode status = UA_Server_run(server, &running);
    printf("OPC UA Server started...\n");
    UA_Server_runUntilInterrupt(server);
    UA_Server_delete(server);
    return 0;
}