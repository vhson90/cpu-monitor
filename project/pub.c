#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "MQTTClient.h"
/*
Broker: broker.emqx.io
TCP Port: 1883 
*/
#define ADDRESS     "tcp://broker.emqx.io:1883"
#define CLIENTID    "publisher_demo"
// #define SUB_TOPIC   "test/topic1"
#define PUB_TOPIC   "cdt/topic1"
#define PUB_TOPIC_2   "cdt/topic2"

// #define QOS         1

void publish(MQTTClient client, char* topic, char* payload) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(pubmsg.payload);
    pubmsg.qos = 1;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
    printf("Message '%s' with delivery token %d delivered\n", payload, token);
}

// Function to read integer value from a file
int read_value_from_file(const char *filepath) {
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    int value;
    if (fscanf(fp, "%d", &value) != 1) {
        perror("Error reading file");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return value;
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    int rc;  // return code
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
   
    //listen for operation
    int i = 0;
    while(1) {
        char msg_temp [40];
        char msg_batt [40];

        if(i % 1 == 0)
        {
            // Read CPU temperature
            int temp = read_value_from_file("/sys/class/thermal/thermal_zone0/temp");
            // Read SSD temperature
            int temp2 = read_value_from_file("/sys/class/hwmon/hwmon3/temp1_input");
            if (temp != -1 && temp2 != -1) {

                sprintf(msg_temp,"CPU: %.1f°C, SSD: %.1f°C", temp / 1000.0, temp2 / 1000.0);
                publish(client, PUB_TOPIC, msg_temp);
            }
        }
        
        if(i % 3 == 0)
        {
            // Read Current battery charge capacity
            int temp3 = read_value_from_file("/sys/class/power_supply/BAT0/charge_now");
            // Read Current battery voltage
            int temp4 = read_value_from_file("/sys/class/power_supply/BAT0/voltage_now");
            if (temp3 != -1 && temp4 != -1) {
                sprintf(msg_batt,"charge: %.1fmAh, voltage: %.1fV", temp3 / 1000.0, temp4 / 1000000.0);
                publish(client, PUB_TOPIC_2, msg_batt);
            }
        }

        i++;
        sleep(1);
    }
    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    return rc;
}