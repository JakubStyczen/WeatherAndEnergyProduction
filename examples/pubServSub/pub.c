#include <open62541/client.h>
#include <open62541/client_config_default.h>
#include <open62541/plugin/log_stdout.h>
#include <stdio.h>
#include <open62541/client_highlevel.h>
#include <open62541/client_subscriptions.h>
#include <unistd.h>

int main() {
    UA_Client *client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));

    // Połączenie z serwerem
    UA_StatusCode status = UA_Client_connect(client, "opc.tcp://192.168.192.26:4840");
    if (status != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return EXIT_FAILURE;
    }

    // Wysyłanie wartości w pętli
    for (int i = 1; i <= 70; i++) {
        UA_Int32 newValue = i * 10;
        UA_Variant value;
        UA_Variant_setScalar(&value, &newValue, &UA_TYPES[UA_TYPES_INT32]);

        UA_NodeId nodeId = UA_NODEID_STRING(1, "my.variable");
        status = UA_Client_writeValueAttribute(client, nodeId, &value);

        if (status == UA_STATUSCODE_GOOD) {
            printf("Wysłano wartość: %d\n", newValue);
        } else {
            printf("Błąd zapisu wartości\n");
        }
        sleep(0.1);
    }

    UA_Client_disconnect(client);
    UA_Client_delete(client);
    return EXIT_SUCCESS;
}