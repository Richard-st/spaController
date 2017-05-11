#include <ArduinoJson.h>

#include "mqttController.h"
#include "myEEPROM.h"
#include "logger.h"
#include "spaController.h"
#include "thermostat.h"

//-------------------------------
// constructor
//-------------------------------
myThermostat::myThermostat(){
  _iState = THEM_STATE_IDLE;
  return;
}

void  myThermostat::setState(int iState){
   _iState = iState;
   return;
}

int  myThermostat::getState(){
  return  _iState;
}

void  myThermostat::loop(){

  long now = millis();

  //sanity checks on values
  if (_myEEPROM.getThermoIdleTimeMS()< 100)
      _myEEPROM.setThermoIdleTime(36000);

  if (_myEEPROM.getThermoSampleTimeMS()< 100)
      _myEEPROM.setThermoSampleTime(12000);


  switch (_iState){

    case THEM_STATE_OFF:
       break;

    case THEM_STATE_IDLE:

       if (now - _iLastTime > _myEEPROM.getThermoIdleTimeMS()  ) {
           _iLastTime = now;
           _iState = THEM_STATE_SAMPLE;
           String sTopic = String(MQTT_BASE_TOPIC + String(MQTT_SWITCH_SET) + String(MQTT_SET_SPA_PUMP) );
           client.publish(sTopic.c_str(), "on");
       }
       break;

    case THEM_STATE_SAMPLE:

       if (now - _iLastTime > _myEEPROM.getThermoSampleTimeMS()  ) {

           if ( _myEEPROM.getSpaTemp() >  mySpaThermometer.getTemp() ){ //heat up
             _iLastTime = now;
             _iState = THEM_STATE_HEATING;
           }
           else{                                                       // go back to idle
             _iLastTime = now;
             _iState = THEM_STATE_IDLE;
             String sTopic = String(MQTT_BASE_TOPIC + String(MQTT_SWITCH_SET) + String(MQTT_SET_SPA_PUMP) );
             client.publish(sTopic.c_str(), "off");
           }
       }
       break;

    case THEM_STATE_HEATING:
       if ( _myEEPROM.getSpaTemp() <=  mySpaThermometer.getTemp() ){
          _iLastTime = now;
          _iState = THEM_STATE_IDLE;
          String sTopic = String(MQTT_BASE_TOPIC + String(MQTT_SWITCH_SET) + String(MQTT_SET_SPA_PUMP) );
          client.publish(sTopic.c_str(), "off");
       }


    default:
       _iLastTime = now;
       _iState = THEM_STATE_IDLE;
       break;


  }
  return;
}
