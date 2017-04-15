#include <ESP8266WebServer.h>
#include <EEPROM.h>


#include "logger.h" 
#include "spaController.h" 
#include "myEEPROM.h"
#include "mqttController.h"
#include "relay.h"
  
//-------------------------------------------
// Global Variables
//-------------------------------------------

  int               iWebPageState = 0;

  const char*       filler  = "                            ";  
  
  int               iControllerMode=MODE_WORK;

  myEEPROM      _myEEPROM;
  myRelay       spaPumpRelay("SpaPump" );
  myRelay       spaLightsRelay("SpaLights" );
  myRelay       spaBubblesRelay("SpaBubbles" );  
  myThermometer mySpaThermometer("spaTemp",ONE_WIRE_SPA_BUS);
  myThermostat  mySpaThermostat;
  
  bool              bRelaySwitches = true;  

  WiFiClient        espClient;
  PubSubClient      client(espClient);
  
  int               iMQTTConnectAttempts;


// Setup Webserver variable

const char *SetupSsid = "IOT_TERMOMETER_SETUP";

//String sPageHtml = "<!DOCTYPE html><html lang='en'><head><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Thermometer Setup</title><style type='text/css'> html, body { height: 100%; margin: 0; font-family: 'Helvetica','Arial', 'Helvetica'; width:100%; } header { background-color:black; color:white; font-size: 35px; text-align:center; padding:5px; } section{ } .content-box{ height:200px; min-width:400px; width:33%; float:left; border-color:black border-collapse: collapse; border-style: solid; border-width: 1px; } .box-title{ width: 100%; font-size: 16px; font-weight: bold; float:left; text-align:center; padding:5px; margin-top:15px; margin-bottom:20px; } .box-line{ width:100%; float:left; margin-bottom:5px; } .box-text{ max-width: 150px; min-width: 150px; font-size: 14px; float:left; padding-right:10px; text-align:right; } .box-entry{ max-width: 50px; min-width: 50px; font-size: 14px; float:left; } footer { background-color:black; color:white; clear:both; text-align:center; padding:5px; } @media screen and (max-width: 400px) { .content-box{ background: white; height:180px; } .box-title{ margin-top:5px; margin-bottom:10px; } header { font-size: 25px; } } </style><!--[if IE]> <script src='http://html5shim.googlecode.com/svn/trunk/html5.js'></script> <![endif]--></head><body><header> ESP8266 Setup </header><form method='post'><section><div class='content-box'><div class='box-title'> Wifi Configuration </div><div class='box-line'><div class='box-text'> Select Wifi Network </div><div class='box-entry'>~~ssidlist</div></div><div class='box-line'><div class='box-text'> Password </div><div class='box-entry'><input type='text' name='password' value='~~password'></div></div></div><div class='content-box'><div class='box-title'> Enter MQTT Server </div><div class='box-line'><div class='box-text'> Server </div><div class='box-entry'><input type='text' name='mqtt-server' value='~~mqtt-server'></div></div><div class='box-line'><div class='box-text'> Port </div><div class='box-entry'><input type='text' name='mqtt-port' value='~~mqtt-port'><br></div></div><div class='box-line'><div class='box-text'> Topic </div><div class='box-entry'><input type='text' name='mqtt-topic' value='~~mqtt-topic'><br></div></div></div><div class='content-box'><div class='box-title'> Control Settings </div><div class='box-line'><div class='box-text'> Max Wifi Attempts </div><div class='box-entry'><input type='text' name='Max-Wifi' value='~~Max-Wifi'></div></div><div class='box-line'><div class='box-text'> Max MQTT Attempts </div><div class='box-entry'><input type='text' name='Max-MQTT' value='~~Max-MQTT'></div></div><div class='box-line'><div class='box-text'> Debug Led </div><div class='box-entry'><input type='radio' name='debug-led' value='debug-on' ~~parm-debug-on > On <input type='radio' name='debug-led' value='debug-off' ~~parm-debug-off > Off </div></div></div><!-- <input type='hidden' name='submitted' value='yes'> --><!-- <br><input type='submit' value='Save Changes and Restart'> --></section><footer><button type='submit' name='formAction' value='save'> Save </button><button type='submit' name='formAction' value='exit'> Exit </button></footer></form></body></html>";
String sPageHtml = "<!DOCTYPE html><html lang='en'><head><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Thermometer Setup</title><style type='text/css'> html, body { height: 100%; margin: 0; font-family: 'Helvetica','Arial', 'Helvetica'; width:100%; } header { background-color:black; color:white; font-size: 35px; text-align:center; padding:5px; } section{ } .content-box{ height:200px; min-width:400px; width:33%; float:left; border-color:black border-collapse: collapse; border-style: solid; border-width: 1px; } .box-title{ width: 100%; font-size: 16px; font-weight: bold; float:left; text-align:center; padding:5px; margin-top:15px; margin-bottom:20px; } .box-line{ width:100%; float:left; margin-bottom:5px; } .box-text{ max-width: 150px; min-width: 150px; font-size: 14px; float:left; padding-right:10px; text-align:right; } .box-entry{ max-width: 50px; min-width: 50px; font-size: 14px; float:left; } footer { background-color:black; color:white; clear:both; text-align:center; padding:5px; } @media screen and (max-width: 400px) { .content-box{ background: white; height:180px; } .box-title{ margin-top:5px; margin-bottom:10px; } header { font-size: 25px; } } </style><!--[if IE]> <script src='http://html5shim.googlecode.com/svn/trunk/html5.js'></script> <![endif]--></head><body><header> ESP8266 Setup </header><form method='post'><section><div class='content-box'><div class='box-title'> Wifi Configuration </div><div class='box-line'><div class='box-text'> Select Wifi Network </div><div class='box-entry'>~~ssidlist</div></div><div class='box-line'><div class='box-text'> Password </div><div class='box-entry'><input type='text' name='password' value='~~password'></div></div></div><div class='content-box'><div class='box-title'> Enter MQTT Server </div><div class='box-line'><div class='box-text'> Server </div><div class='box-entry'><input type='text' name='mqtt-server' value='~~mqtt-server'></div></div><div class='box-line'><div class='box-text'> Port </div><div class='box-entry'><input type='text' name='mqtt-port' value='~~mqtt-port'><br></div></div></div><div class='content-box'><div class='box-title'> Control Settings </div><div class='box-line'><div class='box-text'> Max Wifi Attempts </div><div class='box-entry'><input type='text' name='Max-Wifi' value='~~Max-Wifi'></div></div><div class='box-line'><div class='box-text'> Max MQTT Attempts </div><div class='box-entry'><input type='text' name='Max-MQTT' value='~~Max-MQTT'></div></div><div class='box-line'><div class='box-text'> Debug Led </div><div class='box-entry'><input type='radio' name='debug-led' value='debug-on' ~~parm-debug-on > On <input type='radio' name='debug-led' value='debug-off' ~~parm-debug-off > Off </div></div></div><!-- <input type='hidden' name='submitted' value='yes'> --><!-- <br><input type='submit' value='Save Changes and Restart'> --></section><footer><button type='submit' name='formAction' value='save'> Save </button><button type='submit' name='formAction' value='exit'> Exit </button></footer></form></body></html>";
ESP8266WebServer server(80);

String sWIFINetworks;


//-------------------------------------------
// MQTT Vars
//-------------------------------------------
 
  char mqtt_server[100];

// function declarations
void work_mode();


//-------------------------------------------


//-------------------------------------------
void debug_flash(int noOfFlashes) {
//-------------------------------------------
  int i;
  
  pinMode(DEBUG_LED_PIN, OUTPUT);
  
  for (i=1;i<=noOfFlashes;i++){

    digitalWrite(DEBUG_LED_PIN, HIGH);
    delay(200);  
    digitalWrite(DEBUG_LED_PIN, LOW); 
    delay(200);      
  } 
  delay(300);       
}
//-------------------------------------------



//-------------------------------------------
int checkSetupMode(){
//-------------------------------------------
  //
  // detect if mode switch is requested
  //
  // if MODE_SWITCH_PIN high - continue working mode
  // if MODE_SWITCH_PIN low - go into setup mode after 3 seconds of remaining low
  //
  // returns
  //
  // MODE_WORK 0
  // MODE_SETUP 1
  //
  
  pinMode(MODE_SWITCH_PIN, INPUT);
  
  if (digitalRead(MODE_SWITCH_PIN)==HIGH) return MODE_WORK;

  // pin 13 most be low
  //
  // wait 3 seconds and try again
  //
  
  delay (3000);
  if (digitalRead(MODE_SWITCH_PIN)==HIGH) return MODE_WORK;

  //is MODE_SWITCH_PIN remain low for 3 seconds, go into setup mode
  
  debug_flash(DEBUG_FLASH_ENTER_SETUP);

  return MODE_SETUP;
}

//-------------------------------------------
// Connect to MQTT Server
//-------------------------------------------
void mqtt_connect() {

  
  iMQTTConnectAttempts = 1;


  // Loop until we're reconnected
    while (!client.connected()) {
      //debug_flash(DEBUG_FLASH_MQTT_CONNECT);

      logger_print("Attempting MQTT connection [Server:");
      logger_print(_myEEPROM.getServer());
      logger_print(" ] [Port:");
      logger_print(_myEEPROM.getPort());
      logger_print(" ] ");
    
      // Attempt to connect
      if (client.connect("local")) {
        delay(10);
        logger_println("connected");
        client.subscribe( String( String(MQTT_BASE_TOPIC) + String("#") ).c_str());        
        
    } else {
      logger_print("failed, rc=");
      logger_print(client.state());
      logger_println(" try again in .5 seconds");
      // Wait 1 seconds before retrying
      iMQTTConnectAttempts++;
      delay(100);
    }
  }
}


//-------------------------------------------
// Connect to WIFI
//-------------------------------------------
void wifi_connect() {

  logger_println("Connecting to wifi");


  WiFi.begin( _myEEPROM.getSSID().c_str(), _myEEPROM.getPassword().c_str());
  while (WiFi.status() != WL_CONNECTED) {
    debug_flash(DEBUG_FLASH_WIFI_CONNECT);
    delay(100);
    logger_print(".");
  }
  logger_println("");
  logger_print("Connected to ");
  logger_println(_myEEPROM.getSSID());
  logger_print("IP address: ");
  logger_println(WiFi.localIP());
}

//-------------------------------------------
// build Web Page for setup mode
//-------------------------------------------
void getWifiList(){

  bool bSelectedMatch = false;
  
  //start up html page //
  sWIFINetworks = "<select  name='ssidlist'  >";
   
  logger_println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  logger_println("scan done");
  if (n == 0)
     sWIFINetworks +=("no networks found");
  else
  {
    sWIFINetworks +=(n);
    sWIFINetworks += (" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      sWIFINetworks += "<option value='" ;
      sWIFINetworks +=(WiFi.SSID(i));
      sWIFINetworks += "'";
      if (String (WiFi.SSID(i)) == _myEEPROM.getSSID() )sWIFINetworks += " selected";
      sWIFINetworks += ">";
      sWIFINetworks +=(WiFi.SSID(i));
      sWIFINetworks += "</option>";
      
      sWIFINetworks.replace("Poppy's","Poppy&#x27s");
      
      delay(10);
    }
  }
  //Finish html page //
  sWIFINetworks += "</select> ";
}

//-------------------------------------------
//  process return from web page
//-------------------------------------------
void handleRoot() {

  String sSubmitted = "No";
  int iEEPROMPointer = 0;

  String sPageHtmlWorking = sPageHtml;

  logger_println("handleRoot");


 
  switch (iWebPageState) {

    case WEBPAGE_STATE_INIT :
         logger_println("WEBPAGE_STATE_INIT"); 
       //  server.send(200, "text/html", sPageHtml.c_str() );
         iWebPageState = WEBPAGE_STATE_NORMAL;      
         break;         
    
    case WEBPAGE_STATE_NORMAL :
         logger_println("WEBPAGE_STATE_NORMAL"); 
    
         if ( server.arg("formAction") == "save")

              _myEEPROM.setSSID            (server.arg("ssidlist") );
              _myEEPROM.setPassword        (server.arg("password") );
              _myEEPROM.setServer          (server.arg("mqtt-server") );
              _myEEPROM.setPort            (server.arg("mqtt-port").toInt() );
              _myEEPROM.setMaxWifiAttempts (server.arg("Max-Wifi").toInt() );
              _myEEPROM.setMaxMqttAttempts (server.arg("Max-MQTT").toInt() );
              if ( String(server.arg("debug-led")) == String("debug-on") )
                 _myEEPROM.setDebug        (true);
              else
                 _myEEPROM.setDebug        (false);
              _myEEPROM.burn();
              logger_println("Save Pressed"); 
    
         if ( server.arg("formAction") == "exit"){
              _myEEPROM.setSSID            (server.arg("ssidlist") );
              _myEEPROM.setPassword        (server.arg("password") );
              _myEEPROM.setServer          (server.arg("mqtt-server") );
              _myEEPROM.setPort            (server.arg("mqtt-port").toInt() );
              _myEEPROM.setMaxWifiAttempts (server.arg("Max-Wifi").toInt() );
              _myEEPROM.setMaxMqttAttempts (server.arg("Max-MQTT").toInt() );
              _myEEPROM.setDebug           (server.arg("debug-led").toInt() );
              _myEEPROM.burn();
              logger_println("Exit Pressed");
              work_mode(); 
         }
         break;
         
    default:
         break;
         
  }


  // populate Page from EEPROM

  _myEEPROM.fetch(); 

  getWifiList();
  sPageHtmlWorking.replace("~~ssidlist"    , sWIFINetworks.c_str());    //substitute WIFIs into html
  sPageHtmlWorking.replace("~~password"    ,_myEEPROM.getPassword() ); 
  sPageHtmlWorking.replace("~~mqtt-server" ,_myEEPROM.getServer() ); 
  sPageHtmlWorking.replace("~~mqtt-port"   ,String(_myEEPROM.getPort())  ); 
  sPageHtmlWorking.replace("~~Max-Wifi"    ,String(_myEEPROM.getMaxWifiAttempts()) );  
  sPageHtmlWorking.replace("~~Max-MQTT"    ,String(_myEEPROM.getMaxMqttAttempts()) );
  if (_myEEPROM.getDebug() == true){
        sPageHtmlWorking.replace("~~parm-debug-on","checked");
        sPageHtmlWorking.replace("~~parm-debug-off","");
  }
  else{
        sPageHtmlWorking.replace("~~parm-debug-on","");
        sPageHtmlWorking.replace("~~parm-debug-off","checked");
    
  }

  server.send(200, "text/html", sPageHtmlWorking.c_str() );
  

}



//-------------------------------------------
// Loop work processing
//-------------------------------------------
 void work_mode(){
    //
    // Check WIFI and MQTT client
    // 
    if (WiFi.status() != WL_CONNECTED) {
      wifi_connect();
    }
    if (!client.connected()&& WiFi.status() == WL_CONNECTED ) {
      mqtt_connect();
    }


    client.loop();
    mySpaThermometer.loop();
    mySpaThermostat.loop();

    //thermostat();
 }


//-------------------------------------------
// Setup Webserver to get connection parameters and system settings
//-------------------------------------------
void  setup_mode(){
  logger_println("Configuring access point...");
 
  WiFi.softAP(SetupSsid);

  IPAddress myIP = WiFi.softAPIP();
  logger_print("AP IP address: ");
  logger_println(myIP);
  server.on("/", handleRoot);
  
  server.begin();
  Serial.println("HTTP server started");

  getWifiList();  
}

//-------------------------------------------
// One Off Setup
//-------------------------------------------
void setup() {
    logger_println("**setup() ");  
  
  delay(100); 

  Serial.begin(115200);

  _myEEPROM.fetch();

  iControllerMode = checkSetupMode();

  if ( (iControllerMode == MODE_SETUP ) || ( _myEEPROM.getEEPROMVarsSet()  != 1) ) {
    logger_println("Entering Setup ");
    setup_mode();
  }
  
  strcpy(mqtt_server ,_myEEPROM.getServer().c_str() );
  client.setServer(mqtt_server,_myEEPROM.getPort());
  client.setCallback(callback);    

  //-------------------------------------------
  //setup Relay classes
  //-------------------------------------------
  spaPumpRelay.setPin(RELAY_PUMP);
  spaLightsRelay.setPin(RELAY_LIGHTS);
  spaBubblesRelay.setPin(RELAY_BUBBLES);
  
  
}

 

//-------------------------------------------
// Control Loop
//-------------------------------------------
void loop() {
  
  if (iControllerMode == MODE_SETUP){
    server.handleClient();  //run web server
  }else{
    work_mode();
  }
  


}
