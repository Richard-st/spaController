// Header
#ifndef spaController_h
#define spaController_h


//Globals
#include <WiFiClient.h> 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "myEEPROM.h"
#include "relay.h"
#include "thermometer.h"
#include "thermostat.h"

extern  WiFiClient        espClient;
extern  PubSubClient      client;

extern myEEPROM      _myEEPROM;
extern myRelay       spaPumpRelay;
extern myRelay       spaLightsRelay;
extern myRelay       spaBubblesRelay;
extern myThermometer mySpaThermometer;
extern myThermostat  mySpaThermostat;

extern bool              bRelaySwitches;

void post_temperature();
 
//-------------------------------------------
// Program Definitions
//-------------------------------------------


  // Debug Flasher
  #define DEBUG_LED_PIN 5
  #define DEBUG_FLASH_MAIN_LOOP 1
  #define DEBUG_FLASH_ENTER_SETUP 2
  #define DEBUG_FLASH_WIFI_CONNECT 3
  #define DEBUG_FLASH_MQTT_CONNECT 4
  #define DEBUG_FLASH_SETUP 5
  #define DEBUG_CHANGE_RELAY 6
  #define DEBUG_FLASH_SLEEP 10      

  #define MODE_SWITCH_PIN 13 
  #define MODE_WORK 0
  #define MODE_SETUP 1  

  #define WEBPAGE_STATE_INIT 0
  #define WEBPAGE_STATE_NORMAL 1

  #define RELAY_PUMP    4  
  #define RELAY_BUBBLES 12  
  #define RELAY_LIGHTS  14  

  #define ONE_WIRE_SPA_BUS 2  // DS18B20 pin

#endif
