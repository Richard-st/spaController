// Header
#ifndef thermometer_h
#define thermometer_h

#include <Arduino.h>


class myThermometer
{
  public:
    myThermometer(String sName, int iBus);

	int    getBus();
	
	
    void   setPollTimeMS(int iPollTime);
    int    getPollTimeMS();

	
    float  getTemp();
    float  getTempAndPost();

    void   loop();
	
  private:
  
	void   readThermometer();
  
    String _sThermometerName;
    int    _iPollTime;
	int    _iLastPost = 0;
    float  _fTemp;
	int    _iThermometerBus;


};
#endif


