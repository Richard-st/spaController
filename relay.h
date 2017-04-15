// Header
#ifndef relay_h
#define relay_h

#include <Arduino.h>

#define RELAY_ON  1
#define RELAY_OFF 0

class myRelay
{
  public:
    myRelay(String sName);

    void   setState(int iState);
	int    getState();

    void   setPin(int iPin);
	int    getPin();

	String getName();
	
  private:

     String _sRelayName;
     int _iRelayPin;

};
#endif


