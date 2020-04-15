#ifndef MQTT_H_
#define MQTT_H_
#include "MQTTClient.h"

class MQTT{
	public:
	enum Type {
		PUBLISHER 		= 0,
		SUBSCRIBER 		= 1
	};
private: 
volatile MQTTClient_deliveryToken deliveredtoken;
const char* ADDRESS;
const char* CLIENTID;  
const char* AUTHMETHOD; 
const char* AUTHTOKEN;
const char* TOPIC;
int QOS;
long int TIMEOUT; 
public:

	MQTT(const char* ADDRESS,const char* CLIENTID  ,const char* AUTHMETHOD ,const char* AUTHTOKEN ,const char* TOPIC,int QOS,long int TIMEOUT  ){
	this->ADDRESS= ADDRESS;
	this->CLIENTID= CLIENTID;
	this->AUTHMETHOD= AUTHMETHOD;
	this->AUTHTOKEN= AUTHTOKEN;
	this->TOPIC= TOPIC;
	this->QOS= QOS;
	this->TIMEOUT= TIMEOUT;
	}
	static void delivered(void *context, MQTTClient_deliveryToken dt);
	static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) ;
	static void connlost(void *context, char *cause) ;
	void EstablishConnection(MQTT::Type type,const char * jsonPayload="");
	virtual ~MQTT();
};
#endif
