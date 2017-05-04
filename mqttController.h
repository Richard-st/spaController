#ifndef mqttController_h
#define mqttController_h
#include <arduino.h>

//level 1
#define MQTT_BASE_LOGGER             "home/logger/"

//level 1
#define MQTT_BASE_TOPIC              "home/poolSpa/"

//level 2
#define MQTT_SWITCH_ACK              "switchAck/"            //acknowledge a change in switch state
//level 3
#define MQTT_ACK_SPA_BUBBLES         "ackSpaBubbles/"        //switch to acknowledge
#define MQTT_ACK_SPA_LIGHTS          "ackSpaLights/"         //switch to acknowledge
#define MQTT_ACK_SPA_PUMP            "ackSpaPump/"           //switch to acknowledge

//level 2
#define MQTT_SWITCH_SET              "switchSet/"           //request a change in switch state
//level 3
#define MQTT_SET_SPA_BUBBLES         "setSpaBubbles"        //request to change switch
#define MQTT_SET_SPA_LIGHTS          "setSpaLights"         //request to change switch
#define MQTT_SET_SPA_PUMP            "setSpaPump"           //request to change switch


//level 2
#define MQTT_TEMP_SENSOR             "tempSensor/"          //post Temprature

//level 2
#define MQTT_CONTROLLER_REQ          "controllerReq/"        //request to/from the controller
#define MQTT_CONTROLLER_ACK          "controllerAck/"        //acknowledge controller request
//level 3
#define MQTT_ON_OFF                  "OnOff/"               //on off switch
#define MQTT_GET_STATUS              "getStatus/"            //request controller status
#define MQTT_SET_THERMO_IDLE_TIME    "setThermoIdleTime/"    //Time thermostat wait between temp samples
#define MQTT_SET_THERMO_SAMPLE_TIME  "setThermoSampleTime/"  //Time thermostat sample a temp reading
#define MQTT_SET_SPA_TEMP            "setSpaTemp/"           //Target Spa Temprature
#define MQTT_SET_THERM_POLL_TIME     "setThermPollTime/"     //Time between thermomete temprature readings
#define MQTT_SET_THERMO_STATE        "setThermoState/"       //Change thermostat state (off|idle|sample|heating|



void callback(char* topic, byte* payload, unsigned int length);
void callback_controller(String sTopic, String sBaseTopic, String sPayload  );
void callback_relays(String sTopic, String sBaseTopic, String sPayload  );


#endif
