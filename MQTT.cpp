#include<iostream>
#include<time.h>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTT.h"
using namespace std;
volatile MQTTClient_deliveryToken deliveredtoken;
void delivered(void *context, MQTTClient_deliveryToken dt){
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
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

void connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

void MQTT::EstablishConnection(MQTT::Type type) {
   
   this->type=type;
   MQTTClient_connectOptions opts= MQTTClient_connectOptions_initializer;
   
   
   MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_DEFAULT, NULL);
   opts.keepAliveInterval = 20;
   opts.cleansession = 1;
   opts.username = AUTHMETHOD;
   opts.password = AUTHTOKEN;
   if(MQTT::PUBLISHER==type){
   MQTTClient_willOptions will= MQTTClient_willOptions_initializer;
   will.topicName=TOPIC;
   will.message =LWT;
   //will.qos=QOS;
   opts.will=&will;
}
   int rc;
   
   if(MQTT::PUBLISHER!=type){
	MQTTClient_setCallbacks(client, NULL,connlost, msgarrvd,delivered);
	}
   
   if ((rc = MQTTClient_connect(this->client, &opts)) != MQTTCLIENT_SUCCESS) {
      cout << "Failed to connect, return code " << rc << endl;
      exit(-1); 
   }
}
void MQTT::messageROS(const char * jsonPayload){
   int ch; 
   if(MQTT::PUBLISHER!=this->type){
		printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
		MQTTClient_subscribe(this->client, TOPIC, QOS);

		do{
			ch = getchar();
		}while(ch!='Q' && ch != 'q');
	}
	
   else{
	   MQTTClient_message pubmsg = MQTTClient_message_initializer;
	   MQTTClient_deliveryToken token;
	   char str_payload[150];
	   sprintf(str_payload,jsonPayload);
	   pubmsg.payload = str_payload;
	   pubmsg.payloadlen = strlen(str_payload);
	   pubmsg.qos = QOS;
	   MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
	   cout << "Waiting for up to " << (int)(TIMEOUT/1000) <<
			" seconds for publication of " << str_payload <<
			" \non topic " << TOPIC << " for ClientID: " << CLIENTID << endl;
	   MQTTClient_waitForCompletion(client, token, TIMEOUT);
	   cout << "Message with token " << (int)token << " delivered." << endl;
   }

}
MQTT::~MQTT(){
	cout<<"Ending Connection\n";
	MQTTClient_disconnect(this->client, 10000);
    MQTTClient_destroy(&(this->client));
}
