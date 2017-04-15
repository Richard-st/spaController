#ifndef logger_h
#define logger_h

#include <arduino.h>
#include "mqttController.h"

void logger_print(String cMessage);

void logger_print(int iMessage);

void logger_println(String cMessage);

void logger_println(int iMessage);


#endif