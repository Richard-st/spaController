#ifndef thermostat_h
#define thermostat_h

#include <arduino.h>

#define THEM_STATE_OFF     0
#define THEM_STATE_IDLE    1
#define THEM_STATE_SAMPLE  2
#define THEM_STATE_HEATING 3

class myThermostat
{
  public:
    myThermostat();

    void setState(int iState);
    int  getState();

    void   loop();
  
  private:

    int _iState;
    int _iLastTime = 0;

};


#endif
