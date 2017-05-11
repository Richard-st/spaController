#include "myEEPROM.h"
#include <Arduino.h>
#include "spaController.h"
#include "logger.h"


//-------------------------------
// constructor
//-------------------------------
myEEPROM::myEEPROM(){

}


//-------------------------------
// _iEEPROMVarsSet
//-------------------------------
bool   myEEPROM::getEEPROMVarsSet(){
  return _iEEPROMVarsSet;
  }


//-------------------------------
// SSID
//-------------------------------
void   myEEPROM::setSSID(String sSSID){
  strcpy(_wifiFromEEPROM.cSSid,sSSID.c_str());}

String myEEPROM::getSSID(){
  return String(_wifiFromEEPROM.cSSid);}

//-------------------------------
// Password
//-------------------------------
void   myEEPROM::setPassword(String sPassword){
  strcpy(_wifiFromEEPROM.cPassword,sPassword.c_str());}

String myEEPROM::getPassword(){
  return String(_wifiFromEEPROM.cPassword);}

//-------------------------------
// Server
//-------------------------------
void   myEEPROM::setServer(String sServer){
  strcpy(_mqttFromEEPROM.cServer,sServer.c_str());}

String myEEPROM::getServer(){
  return String(_mqttFromEEPROM.cServer);}

//-------------------------------
// Port
//-------------------------------
void   myEEPROM::setPort(int iPort){
  _mqttFromEEPROM.iPort = iPort;}

int  myEEPROM::getPort(){
  return _mqttFromEEPROM.iPort;}

//-------------------------------
// Topic
//-------------------------------
void   myEEPROM::setTopic(String sTopic){
  strcpy(_mqttFromEEPROM.cTopic,sTopic.c_str());}

String myEEPROM::getTopic(){
  return String(_mqttFromEEPROM.cTopic);}

//-------------------------------
// MaxWifiAttempts
//-------------------------------
void   myEEPROM::setMaxWifiAttempts(int iMaxWifiAttempts){
  _controlFromEEPROM.iMaxWifiAttempts = iMaxWifiAttempts;}

int  myEEPROM::getMaxWifiAttempts(){
  return _controlFromEEPROM.iMaxWifiAttempts;}

//-------------------------------
// MaxMqttAttempts
//-------------------------------
void   myEEPROM::setMaxMqttAttempts(int iMaxMqttAttempts){
  _controlFromEEPROM.iMaxMqttAttempts = iMaxMqttAttempts;}

int  myEEPROM::getMaxMqttAttempts(){
  return _controlFromEEPROM.iMaxMqttAttempts;}

//-------------------------------
// Debug
//-------------------------------
void   myEEPROM::setDebug(bool bDebug){
  _controlFromEEPROM.bDebug = bDebug;}

bool  myEEPROM::getDebug(){
  return _controlFromEEPROM.bDebug;}

//--------------------------------------------------
// iThermoIdleTime - how oftern we poll for a temp
//--------------------------------------------------

void    myEEPROM::setThermoIdleTime(int iThermoIdleTime){
  _controlFromEEPROM.iThermoIdleTime = iThermoIdleTime;}

int     myEEPROM::getThermoIdleTime(){
  return _controlFromEEPROM.iThermoIdleTime;}

  int     myEEPROM::getThermoIdleTimeMS(){
    return _controlFromEEPROM.iThermoIdleTime*1000;}

//--------------------------------------------------
// SpaWaitTime - second to wait before turning pump on for temp check
//--------------------------------------------------

void    myEEPROM::setThermoSampleTime(int iThermoSampleTime){
  _controlFromEEPROM.iThermoSampleTime = iThermoSampleTime;}

int     myEEPROM::getThermoSampleTime(){
  return _controlFromEEPROM.iThermoSampleTime;}

  int     myEEPROM::getThermoSampleTimeMS(){
    return _controlFromEEPROM.iThermoSampleTime*1000;}

//--------------------------------------------------
// spaTemp - temp settnig for spa
//--------------------------------------------------

void    myEEPROM::setSpaTemp(int iSpaTemp){
  _controlFromEEPROM.iSpaTemp = iSpaTemp;}

int     myEEPROM::getSpaTemp(){
  return _controlFromEEPROM.iSpaTemp;}

//--------------------------------------------------
// Thermometer polling interval
//--------------------------------------------------
void   myEEPROM::setThermPollTime(int iThermPollTime){
  _controlFromEEPROM.iThermPollTime = iThermPollTime;}

int   myEEPROM::getThermPollTime(){
  return _controlFromEEPROM.iThermPollTime;}

  int   myEEPROM::getThermPollTimeMS(){
    return _controlFromEEPROM.iThermPollTime*1000;}

//--------------------------------------------------
// On OFF switch
//--------------------------------------------------
void   myEEPROM::setOnButton(bool bOn){
  _controlFromEEPROM.bOn = bOn;}

bool   myEEPROM::getOnButton(){
  return _controlFromEEPROM.bOn;}


//-------------------------------
// burn EEPROM
//-------------------------------
void myEEPROM::burn()
{
  //setup Memmap

  _memMapFromEEPROM.iAddrMemMap  = 1; //position zero is used for data present flag
  _memMapFromEEPROM.iLenMemMap   = sizeof(_memMapFromEEPROM);

  _memMapFromEEPROM.iAddrWifi    = _memMapFromEEPROM.iAddrMemMap + _memMapFromEEPROM.iLenMemMap;
  _memMapFromEEPROM.iLenWifi     = sizeof(_wifiFromEEPROM);

  _memMapFromEEPROM.iAddrMQTT    = _memMapFromEEPROM.iAddrWifi + _memMapFromEEPROM.iLenWifi;
  _memMapFromEEPROM.iLenMqtt     = sizeof(_mqttFromEEPROM);

  _memMapFromEEPROM.iAddrControl = _memMapFromEEPROM.iAddrMQTT + _memMapFromEEPROM.iLenMqtt;
  _memMapFromEEPROM.iLenControl  = sizeof(_controlFromEEPROM);

  debug("  EEPROM BEFORE WRITE");

  EEPROM.begin(512);

  EEPROM.put(0                              , true);
  EEPROM.put(1                              , _memMapFromEEPROM);
  EEPROM.put(_memMapFromEEPROM.iAddrWifi    , _wifiFromEEPROM);
  EEPROM.put(_memMapFromEEPROM.iAddrMQTT    , _mqttFromEEPROM);
  EEPROM.put(_memMapFromEEPROM.iAddrControl , _controlFromEEPROM);
  EEPROM.end();
}

//-------------------------------
// burn EEPROM
//-------------------------------
void myEEPROM::fetch()
{
  EEPROM.begin(512);
  EEPROM.get(0                              , _iEEPROMVarsSet);
  EEPROM.get(1                              , _memMapFromEEPROM);
  EEPROM.get(_memMapFromEEPROM.iAddrWifi    , _wifiFromEEPROM);
  EEPROM.get(_memMapFromEEPROM.iAddrMQTT    , _mqttFromEEPROM);
  EEPROM.get(_memMapFromEEPROM.iAddrControl , _controlFromEEPROM);
  EEPROM.end();

  debug("  EEPROM AFTER READ");
}



//-------------------------------
// debug Structures
//-------------------------------
void myEEPROM::debug(String sTitle)
{
   logger_println("==============================");
   logger_println(sTitle);
   logger_println("==============================");
   logger_print  (String ("== _iEEPROMVarsSet   - ")) ;
   logger_println(String(_iEEPROMVarsSet) );
   logger_print  (String ("== cSSid             - ")) ;
   logger_println(String(_wifiFromEEPROM.cSSid) );
   logger_print  (String ("== cPassword         - ")) ;
   logger_println(String(_wifiFromEEPROM.cPassword) );
   logger_print  (String ("== cServer           - ")) ;
   logger_println(String(_mqttFromEEPROM.cServer) );
   logger_print  (String ("== iPort             - ")) ;
   logger_println(String(_mqttFromEEPROM.iPort) );
   logger_print  (String ("== cTopic            - ")) ;
   logger_println(String(_mqttFromEEPROM.cTopic) );
   logger_print  (String ("== iMaxWifiAttempts  - ")) ;
   logger_println(String(_controlFromEEPROM.iMaxWifiAttempts) );
   logger_print  (String ("== iMaxMqttAttempts  - ")) ;
   logger_println(String(_controlFromEEPROM.iMaxMqttAttempts) );
   logger_print  (String ("== bDebug            - ")) ;
   logger_println(String(_controlFromEEPROM.bDebug) );
   logger_print  (String ("== iThermoIdleTime   - ")) ;
   logger_println(String(_controlFromEEPROM.iThermoIdleTime) );
   logger_print  (String ("== iThermoSampleTime - ")) ;
   logger_println(String(_controlFromEEPROM.iThermoSampleTime) );
   logger_print  (String ("== iSpaTemp          - ")) ;
   logger_println(String(_controlFromEEPROM.iSpaTemp) );
   logger_print  (String ("== iThermPollTime    - ")) ;
   logger_println(String(_controlFromEEPROM.iThermPollTime) );


   return;
}
