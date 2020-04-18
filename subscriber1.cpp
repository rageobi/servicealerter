#include <wiringPi.h>
#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
#include<iostream>
#include <json-c/json.h>
#define LedPin 7

#define ADDRESS    "tcp://192.168.0.177:1883"
#define CLIENTID    "sub1"
#define AUTHMETHOD "rageobi"
#define AUTHTOKEN  "llll"
#define TOPIC      "ee513/CPUTemp"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L
using namespace std;
volatile MQTTClient_deliveryToken deliveredtoken;
json_object * data;
void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}
void json_parse(json_object * jobj) {
 json_object_object_foreach(jobj, key, val) {
 enum json_type type = json_object_get_type(val);
 switch (type) {
   case json_type_double: 
   if( strcmp(key,"temp")==0 &&json_object_get_double(val)>30.0){
	   digitalWrite (LedPin, HIGH) ;
	   cout<<"\n"<<key<<" value higher than expected.LED turned ON for three seconds\n";
		delay(3000);cout<<"TURNING OFF LED\n";
		digitalWrite (LedPin, LOW) ;	
   }
   
   break;
   case json_type_null: printf("type: json_type_null, ");
   printf("value is null \n");
   break;
   case json_type_string: 
   //printf("type: json_type_string, ");
    printf("value: %s\n", json_object_get_string(val));
   break;
   case json_type_array: printf("type: json_type_array, ");
   json_parse(val);
   break;
   case json_type_int: printf("type: json_type_int, ");
   printf("value: %d\n", json_object_get_int(val));
   break;
   case json_type_boolean: printf("type: json_type_boolean, ");
   printf("value: %c\n", json_object_get_boolean(val));
   break;
   case json_type_object: printf("type: json_type_object, ");
    json_parse(val);
   break;
 }
 }
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = (char*) message->payload;
    data = json_tokener_parse(payloadptr);
    json_parse(data);
    for(i=0; i<message->payloadlen; i++) {
        putchar(*payloadptr++);
    }
    putchar('\n');
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[]) {
	        if(wiringPiSetup() == -1) { //when initialize wiringPi failed, print message to screen
                printf("setup wiringPi failed !\n");
                return -1;
        }
     pinMode(LedPin, OUTPUT);
    MQTTClient client;
    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_DEFAULT, NULL);
    opts.keepAliveInterval = 20;
    opts.cleansession = 1;
    opts.username = AUTHMETHOD;
    opts.password = AUTHTOKEN;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);
	
    do {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}

