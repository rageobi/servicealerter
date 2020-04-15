#include <json-c/json.h>
#include<iostream>
#include "PI.h"
#include <string.h>
using namespace std;

int main(){
json_object * jobj = json_object_new_object();
json_object *temp = json_object_new_double(getCPUTemperature());
json_object *dateTime = json_object_new_string(getDateTime());
  json_object_object_add(jobj,"temp", temp);
  json_object_object_add(jobj,"Date", dateTime);
   printf ("The json object created:%s",json_object_to_json_string(jobj));
 //printf ("JSON string is: %s\n", dateTime);

	return 1;
}
