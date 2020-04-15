/*
 * application.cpp  Created on: 29 Apr 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring Raspberry Pi"
 * See: www.exploringrpi.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European
 * Union Public License (the "EUPL") version 1.1 as published by the
 * European Union. Any use of this Software, other than as authorized
 * under this License is strictly prohibited (to the extent such use
 * is covered by a right of the copyright holder of this Software).
 *
 * This Software is provided under the License on an "AS IS" basis and
 * without warranties of any kind concerning the Software, including
 * without limitation merchantability, fitness for a particular purpose,
 * absence of defects or errors, accuracy, and non-infringement of
 * intellectual property rights other than copyright. This disclaimer
 * of warranty is an essential part of the License and a condition for
 * the grant of any rights to this Software.
 *
 * For more details, see http://www.derekmolloy.ie/
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "MQTT.h"
#include "ADXL345.h"
#include "AppFunctions.h"

using namespace std;
using namespace exploringRPi;

//Please replace the following address with the address of your server
#define ADDRESS    "tcp://192.168.0.177:1883"
#define CLIENTID   "rpiPublisher"
#define AUTHMETHOD "rageobi"
#define AUTHTOKEN  "llll"
#define TOPIC      "ee513/CPUTemp"
#define QOS        1
#define TIMEOUT    10000L

int main() {
   ADXL345 sensor(1,0x53);
   sensor.setResolution(ADXL345::NORMAL);
   sensor.setRange(ADXL345::PLUSMINUS_4_G);
   sensor.readSensorState();
   MQTT obj= MQTT(ADDRESS,CLIENTID,AUTHMETHOD,AUTHTOKEN,TOPIC,QOS,TIMEOUT);
   const char * jsonPayload= jsonCharacteriser(sensor.getPitch(),sensor.getRoll());
   //cout<<jsonPayload;
   obj.EstablishConnection(MQTT::PUBLISHER,jsonPayload);
}
