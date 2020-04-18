#include "AppFunctions.h"
#include <sstream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <json-c/json.h>
#define  CPU_TEMP "/sys/class/thermal/thermal_zone0/temp"
using namespace std;

float getCPUTemperature() {        // get the CPU temperature
   int cpuTemp;                    // store as an int
   fstream fs;
   fs.open(CPU_TEMP, fstream::in); // read from the file
   fs >> cpuTemp;
   fs.close();
   return (((float)cpuTemp)/1000);
}

char* getDateTime(){
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  char *time = (char *)malloc(8);
  sprintf(time, "%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec);
  //if (time[strlen(time)-1] == '\n') time[strlen(time)-1] = '\0';
  return time;
}

const char * jsonCharacteriser(float pitchVal, float rollVal){
   json_object * jobj = json_object_new_object();
   json_object * jobj2 = json_object_new_object();
   
   json_object *temp = json_object_new_double(getCPUTemperature());
   json_object *roll = json_object_new_double(rollVal);
   json_object *pitch = json_object_new_double(pitchVal);
   json_object *dateTime = json_object_new_string(getDateTime());
   
   json_object_object_add(jobj,"temp", temp);
   json_object_object_add(jobj,"time", dateTime);
   json_object_object_add(jobj,"pitch", pitch);
   json_object_object_add(jobj,"roll", roll);
   
   json_object_object_add(jobj2,"d", jobj);
   return json_object_to_json_string(jobj2);
}
