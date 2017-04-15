#include <Arduino.h>
#include "spaController.h"
#include "logger.h"
#include "thermometer.h"
#include "mqttController.h"

#include <OneWire.h>
#include <DallasTemperature.h>

   OneWire           *oneWire;
   DallasTemperature *DS18B20;  

//-------------------------------
// constructor
//-------------------------------
myThermometer::myThermometer(String sName, int iBus){

  _sThermometerName = sName;

  oneWire = new OneWire(iBus);
  DS18B20 = new DallasTemperature(oneWire); 

}


//-------------------------------
// getPin
//-------------------------------
int    myThermometer::getBus(){
	return _iThermometerBus;
}

//-------------------------------
// setPollTime
//-------------------------------

void myThermometer::setPollTimeMS(int iPollTime){
	_iPollTime =  iPollTime;
	return;
}

//-------------------------------
// getPollTime
//-------------------------------

int myThermometer::getPollTimeMS(){
	return _iPollTime;
}

//-------------------------------
// getTemp
//-------------------------------
float  myThermometer::getTemp(){
	 readThermometer();
	 return _fTemp;
}
//-------------------------------
// getTempAndPost
//-------------------------------	
float  myThermometer::getTempAndPost(){
	
 	 readThermometer();
	 
	 String sTopic = String(MQTT_BASE_TOPIC + String(MQTT_TEMP_SENSOR) + _sThermometerName ); 	

	 String sTemp(_fTemp,3);
	 
     client.publish(sTopic.c_str(), sTemp.c_str());

	  
	 return _fTemp;
}

//---------------------------------------------------
// loop - posts the temprature after setPollTimeMS
//
//----------------------------------------------------	
void myThermometer::loop(){
	
    long now = millis(); 

    //sanity checks on values
    if (_myEEPROM.getThermPollTime()< 100)
        _myEEPROM.setThermPollTime(2000);

   
    if (now - _iLastPost > _myEEPROM.getThermPollTime()  ) {
      _iLastPost = now;
      this->getTempAndPost();   
      delay (2); //stop double message posting
    }	
	
	return ;
}

//-------------------------------
// readThermometer
//-------------------------------
void myThermometer::readThermometer(){


 do {
      DS18B20->requestTemperatures(); 
      _fTemp = DS18B20->getTempCByIndex(0);
    } while (_fTemp == 85.0 || _fTemp == (-127.0));
	
	
}



