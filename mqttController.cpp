#include <ArduinoJson.h>

#include "spaController.h"
#include "mqttController.h"
#include "myEEPROM.h"
#include "logger.h"


//-------------------------------------------
// MQTT Callback Entry Point
//-------------------------------------------
void callback(char* topic, byte* payload, unsigned int length) {
  String sPayload = "";
  String sTopic = topic;
  String sBaseTopic;

  // put payload into sting
  for (int i = 0; i < length; i++) {
    sPayload += (char)payload[i];
  }



  sBaseTopic = MQTT_BASE_TOPIC; 


  logger_print("**MQTT CALLBACK  **");    
  logger_print("  Topic = ");  
  logger_print(sTopic);    
  logger_print("  Payload = ");  
  logger_println(sPayload);    

  if ((sTopic.indexOf(MQTT_SWITCH_SET) > -1) && bRelaySwitches)  {
    callback_relays(sTopic, sBaseTopic, sPayload );
    return; //Set relays
  }

  if (sTopic.indexOf(MQTT_CONTROLLER_REQ) > -1) {
    callback_controller(sTopic, sBaseTopic, sPayload );
    return; //Set set controller
  }
}

//-------------------------------------------
// callback for controller requests
//-------------------------------------------
void callback_controller(String sTopic, String sBaseTopic, String sPayload  ){

  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& JsonRoot = jsonBuffer.createObject();
  
  String sJson;


  //----------------------------------------------//
  // Relay Shutdown Request
  //----------------------------------------------//

  if ( sTopic.indexOf( String(MQTT_CONTROLLER_REQ) + String(MQTT_TURN_OFF) ) > -1){
    logger_println("  Shut Down"); 
    client.publish(String(sBaseTopic +  MQTT_SWITCH_ACK + MQTT_ACK_SPA_BUBBLES).c_str(), "off");
    client.publish(String(sBaseTopic +  MQTT_SWITCH_ACK + MQTT_ACK_SPA_LIGHTS).c_str(), "off");
    client.publish(String(sBaseTopic +  MQTT_SWITCH_ACK + MQTT_ACK_SPA_PUMP).c_str(), "off");
    
    bRelaySwitches = false;
  }

  //----------------------------------------------//
  // Relay Startup Request
  //----------------------------------------------//

  if ( sTopic.indexOf( String(MQTT_CONTROLLER_REQ) + String(MQTT_TURN_ON) ) > -1){
    logger_println("  Start Up"); 
    bRelaySwitches = true;
  }

  
  //----------------------------------------------//
  // getControllerStatus
  //----------------------------------------------//

  if ( sTopic.indexOf( String(MQTT_CONTROLLER_REQ) + String(MQTT_GET_STATUS) ) > -1){  
   JsonRoot["cSSid"]              = _myEEPROM.getSSID();
   JsonRoot["cPassword"]          = _myEEPROM.getPassword();
   JsonRoot["cServer"]            = _myEEPROM.getServer();
   JsonRoot["iPort"]              = _myEEPROM.getPort();
   JsonRoot["cTopic"]             = _myEEPROM.getTopic();
   JsonRoot["iMaxWifiAttempts"]   = _myEEPROM.getMaxWifiAttempts();
   JsonRoot["iMaxMqttAttempts"]   = _myEEPROM.getMaxMqttAttempts();
   JsonRoot["bDebug"]             = _myEEPROM.getDebug();
   JsonRoot["iThermoIdleTime"]    = _myEEPROM.getThermoIdleTime();
   JsonRoot["iThermoSampleTime"]  = _myEEPROM.getThermoSampleTime();
   JsonRoot["iSpaTemp"]           = _myEEPROM.getSpaTemp();
   JsonRoot["iThermPollTime"]     = _myEEPROM.getThermPollTime();

   JsonRoot.printTo(sJson);

   logger_println(sJson);

   client.publish(String(sBaseTopic + MQTT_CONTROLLER_ACK + MQTT_GET_STATUS).c_str(),sJson.c_str() );
  }

  //----------------------------------------------//
  // setSpaIdleTime
  //----------------------------------------------//

  if ( sTopic.indexOf( String(MQTT_CONTROLLER_REQ) + String(MQTT_SET_THERMO_IDLE_TIME ) ) > -1){  
    _myEEPROM.setThermoIdleTime(sPayload.toInt());
    client.publish(String(sBaseTopic + MQTT_CONTROLLER_REQ + MQTT_GET_STATUS).c_str(),"" );
    _myEEPROM.burn();
  }


  //----------------------------------------------//
  // setSpaSampleTime
  //----------------------------------------------//

  if ( sTopic.indexOf( String(MQTT_CONTROLLER_REQ) + String(MQTT_SET_THERMO_SAMPLE_TIME ) ) > -1){  
    _myEEPROM.setThermoSampleTime(sPayload.toInt());
    client.publish(String(sBaseTopic + MQTT_CONTROLLER_REQ + MQTT_GET_STATUS).c_str(),"" );
    _myEEPROM.burn();
  }

  //----------------------------------------------//
  // setSpaTemp
  //----------------------------------------------//

  if ( sTopic.indexOf( String(MQTT_CONTROLLER_REQ) + String(MQTT_SET_SPA_TEMP ) ) > -1){  
    _myEEPROM.setSpaTemp(sPayload.toInt());
    client.publish(String(sBaseTopic + MQTT_CONTROLLER_REQ + MQTT_GET_STATUS).c_str(),"" );
    _myEEPROM.burn();
  }

  //----------------------------------------------//
  // set iThermPollTime
  //----------------------------------------------//

  if ( sTopic.indexOf( String(MQTT_CONTROLLER_REQ) + String(MQTT_SET_THERM_POLL_TIME ) ) > -1){  
    _myEEPROM.setThermPollTime(sPayload.toInt());
    client.publish(String(sBaseTopic + MQTT_CONTROLLER_REQ + MQTT_GET_STATUS).c_str(),"" );
    _myEEPROM.burn();
  }

  //----------------------------------------------//
  // set thermostat state (non EEPROM)
  //----------------------------------------------//

  if ( sTopic.indexOf( String(MQTT_CONTROLLER_REQ) + String(MQTT_SET_THERMO_STATE ) ) > -1){  
    mySpaThermostat.setState(sPayload.toInt());
    client.publish(String(sBaseTopic + MQTT_CONTROLLER_REQ + MQTT_GET_STATUS).c_str(),"" );
  }



}




//-------------------------------------------
// callback for relay requests
//-------------------------------------------
void callback_relays(String sTopic, String sBaseTopic, String sPayload  ){

  logger_print("**CALLBACK RELAYS** "); 
  
  if ( sTopic.indexOf( String(MQTT_SWITCH_SET)+ String(MQTT_SET_SPA_BUBBLES) ) > -1){  
    logger_println(spaBubblesRelay.getName());  

    if (sPayload.equals("on"))
       spaBubblesRelay.setState(HIGH);

    if (sPayload.equals("off"))
       spaBubblesRelay.setState(LOW);

    return;
  }
  
  if ( sTopic.indexOf( String(MQTT_SWITCH_SET)+ String(MQTT_SET_SPA_PUMP) ) > -1){  
    logger_println(spaPumpRelay.getName());  

    if (sPayload.equals("on")){
       spaPumpRelay.setState(HIGH);
    }

    if (sPayload.equals("off")){
       spaPumpRelay.setState(LOW);
    }

    return;
  }
  
  if ( sTopic.indexOf( String(MQTT_SWITCH_SET)+ String(MQTT_SET_SPA_LIGHTS) ) > -1){  
    logger_println(spaLightsRelay.getName());  
    
    if (sPayload.equals("on"))
       spaLightsRelay.setState(HIGH);

    if (sPayload.equals("off"))
       spaLightsRelay.setState(LOW);

    return;
  }
  

  return;


}
