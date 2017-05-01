#include <Arduino.h>
#include "spaController.h"
#include "logger.h"
#include "relay.h"


//-------------------------------
// constructor
//-------------------------------
myRelay::myRelay(String sName){
  _sRelayName = sName;
}


//-------------------------------
// setState
//-------------------------------
void   myRelay::setState(int iSstate){


	  pinMode(_iRelayPin, OUTPUT);

	  String sTopic = String(MQTT_BASE_TOPIC + String("switchAck/ack") + _sRelayName );

  switch (iSstate){

      case RELAY_ON:
	    client.publish(sTopic.c_str(), "on");
		  digitalWrite(_iRelayPin,LOW);
		break;

	  case RELAY_OFF:
	    client.publish(sTopic.c_str(), "off");
		digitalWrite(_iRelayPin,HIGH);
		break;
  }
  return;
}

//-------------------------------
// setState
//-------------------------------
int    myRelay::getState(){

	pinMode(_iRelayPin, INPUT);
	return digitalRead(_iRelayPin);

  }

//-------------------------------
// setPin
//-------------------------------
void   myRelay::setPin(int iPin){
	_iRelayPin = iPin;
	return;
}

//-------------------------------
// getPin
//-------------------------------
int    myRelay::getPin(){
	return _iRelayPin;
}

//-------------------------------
// getName
//-------------------------------

String myRelay::getName(){
	return _sRelayName;
}
