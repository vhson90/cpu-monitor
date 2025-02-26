#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "MQTTClient.h"
#include <mysql/mysql.h>

#define ADDRESS     "tcp://broker.emqx.io:1883"
#define CLIENTID    "subscriber_demo"
#define SUB_TOPIC   "cdt/topic1"
#define SUB_TOPIC_2   "cdt/topic2"
#define SUB_TOPIC_3   "cdt/topic3"
#define SUB_TOPIC_4   "cdt/topic4"
// #define QOS         1

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

char *server = "localhost";
char *user = "root";
char *password = "123456"; /* set me first */
char *database = "Sensors";




int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char* payload = message->payload;
    printf("Received message: %s\n", payload);
    
    conn = mysql_init(NULL);
    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) 
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }  
    float cpu_temp, ssd_temp, charge, voltage;
    if (sscanf(payload, "CPU: %f°C, SSD: %f°C", &cpu_temp, &ssd_temp) == 2) {
        // printf("CPU Temperature: %.1f°C\n", cpu_temp);
        // printf("SSD Temperature: %.1f°C\n", ssd_temp);
        char sql[200];
        sprintf(sql,"insert into temp_sensor(CPU, SSD) values (%.1f, %.1f)",cpu_temp, ssd_temp);
        mysql_query(conn,sql);
    }
     else if (sscanf(payload, "charge: %fmAh, voltage: %fV", &charge, &voltage) == 2)
    {
        char sql[200];
        sprintf(sql,"insert into battery(charge, voltage) values (%.1f, %.1f)",charge, voltage);
        mysql_query(conn,sql);
    }
    else
    {
        printf("Failed to parse message!\n");
    }
    

    
    mysql_close(conn);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);

    int rc;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
   
    //listen for operation
    MQTTClient_subscribe(client, SUB_TOPIC, 0);
    MQTTClient_subscribe(client, SUB_TOPIC_2, 0);
    MQTTClient_subscribe(client, SUB_TOPIC_3, 0);
    MQTTClient_subscribe(client, SUB_TOPIC_4, 0);


    while(1) {
        //send temperature measurement
    }
    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    return rc;
}