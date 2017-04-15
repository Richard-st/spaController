#include "logger.h"
#include "spaController.h"

String _sMessage ="";

void logger_print(String cMessage){
	_sMessage += cMessage.c_str();
  Serial.print(cMessage);
}

void logger_print(int iMessage){
  _sMessage += String(iMessage);
  Serial.print(iMessage);
}


void logger_println(String cMessage){

  _sMessage += cMessage ;     
	client.publish(MQTT_BASE_LOGGER , _sMessage.c_str() );
	_sMessage = "";
  Serial.println(cMessage);
}

void logger_println(int iMessage){
  _sMessage += String(iMessage);     
  client.publish(MQTT_BASE_LOGGER , _sMessage.c_str() );
  _sMessage = "";  
  Serial.println(iMessage);
}
