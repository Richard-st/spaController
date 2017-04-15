// Header
#ifndef myeeprom_h
#define myeeprom_h

#include "EEPROM.h"
#include <Arduino.h>

class myEEPROM
{
  public:
    myEEPROM();

    bool   getEEPROMVarsSet();

    void   setSSID(String sSSID);
    String getSSID();

    void   setPassword(String sPassword);
    String getPassword();

    void   setServer(String sServer);
    String getServer();

    void   setPort(int iPort);
    int    getPort();

    void   setTopic(String sTopic);
    String getTopic();

    void   setMaxWifiAttempts(int iMaxWifiAttempts);
    int    getMaxWifiAttempts();

    void   setMaxMqttAttempts(int iMaxMqttAttempts);
    int getMaxMqttAttempts();

    void   setDebug(bool bDebug);
    bool   getDebug();

    void   setThermoIdleTime(int iThermoIdleTime);
    int    getThermoIdleTime();

    void   setThermoSampleTime(int iThermoSampleTime);
    int    getThermoSampleTime();

    void   setSpaTemp(int iSpaTemp);
    int    getSpaTemp();

    void   setThermPollTime(int iThermPollTime);
    int    getThermPollTime();

    void   burn();
    void   fetch();

    void   debug(String sTitle);


  private:

    bool _iEEPROMVarsSet ;  
  
    struct structEEPROMMemMap{
      int   iAddrMemMap,iLenMemMap ;
      int   iAddrWifi,iLenWifi ;
      int   iAddrMQTT,iLenMqtt;  
      int   iAddrControl,iLenControl;  
    } _memMapFromEEPROM;
 
    struct structWifi{
      char   cSSid[33];
      char   cPassword[20];
    } _wifiFromEEPROM;

    struct structMQTT{
      char   cServer[100];
      int    iPort;
      char   cTopic[100];
    } _mqttFromEEPROM;

    struct structControl{
      int    iMaxWifiAttempts;
      int    iMaxMqttAttempts;
      bool   bDebug;
      int    iThermoIdleTime;   
      int    iThermoSampleTime;
      int    iSpaTemp;
      int    iThermPollTime;
    } _controlFromEEPROM;


};
#endif


