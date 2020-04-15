#include<iostream>
#include<time.h>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTT.h"
using namespace std;
void MQTT::delivered(void *context, MQTTClient_deliveryToken dt){
		
    printf("Message with token value %d delivery confirmed\n", dt);
    //deliveredtoken = dt;
}
int MQTT::msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = (char*) message->payload;
    for(i=0; i<message->payloadlen; i++) {
        putchar(*payloadptr++);
    }
    putchar('\n');
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void MQTT::connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

void MQTT::EstablishConnection(MQTT::Type type,const char * jsonPayload) {
   char str_payload[100];
   int ch;     
   MQTTClient client;
   MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
   MQTTClient_message pubmsg = MQTTClient_message_initializer;
   MQTTClient_deliveryToken token;
   MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
   opts.keepAliveInterval = 20;
   opts.cleansession = 1;
   opts.username = AUTHMETHOD;
   opts.password = AUTHTOKEN;
   int rc;
   
   if(MQTT::PUBLISHER!=type){
	MQTTClient_setCallbacks(client, NULL, MQTT::connlost, MQTT::msgarrvd, MQTT::delivered);
	}
   
   if ((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS) {
      cout << "Failed to connect, return code " << rc << endl;
      exit(-1); 
   }
   if(MQTT::PUBLISHER!=type){
		printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
		MQTTClient_subscribe(client, TOPIC, QOS);

		do{
			ch = getchar();
		}while(ch!='Q' && ch != 'q');
	}
	
   else{
	   sprintf(str_payload,jsonPayload);
	   pubmsg.payload = str_payload;
	   pubmsg.payloadlen = strlen(str_payload);
	   pubmsg.qos = QOS;
	   pubmsg.retained = 0;
	   MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
	   cout << "Waiting for up to " << (int)(TIMEOUT/1000) <<
			" seconds for publication of " << str_payload <<
			" \non topic " << TOPIC << " for ClientID: " << CLIENTID << endl;
	   rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
	   cout << "Message with token " << (int)token << " delivered." << endl;
   }
   
   MQTTClient_disconnect(client, 10000);
   MQTTClient_destroy(&client);

};
MQTT::~MQTT(){
}
