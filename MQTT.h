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
	friend void delivered(void *context, MQTTClient_deliveryToken dt);
	friend int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) ;
	friend void connlost(void *context, char *cause) ;
	const char* ADDRESS;
	const char* CLIENTID;  
	const char* AUTHMETHOD; 
	const char* AUTHTOKEN;
	const char* TOPIC;
	const char* LWT;
	int QOS;
	long int TIMEOUT; 
public:

	MQTT(const char* ADDRESS,const char* CLIENTID  ,const char* AUTHMETHOD ,const char* AUTHTOKEN ,const char* TOPIC,const char* LWT ,int QOS,long int TIMEOUT  ){
	this->ADDRESS= ADDRESS;
	this->CLIENTID= CLIENTID;
	this->AUTHMETHOD= AUTHMETHOD;
	this->AUTHTOKEN= AUTHTOKEN;
	this->TOPIC= TOPIC;
	this->QOS= QOS;
	this->TIMEOUT= TIMEOUT;
	this->LWT=LWT;
	}
	void EstablishConnection(MQTT::Type type,const char * jsonPayload="");
	virtual ~MQTT();
};
void delivered(void *context, MQTTClient_deliveryToken dt);
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) ;
void connlost(void *context, char *cause) ;
#endif
