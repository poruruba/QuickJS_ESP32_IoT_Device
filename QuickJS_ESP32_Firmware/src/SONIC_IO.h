#ifndef __SONIC_IO_H
#define __SONIC_IO_H

#include "Arduino.h"

class SONIC_IO 
{
private:
  uint8_t _trig;
  uint8_t _echo;
public:
  void begin(uint8_t trig = 26, uint8_t echo = 36);
  float getDuration();
  float getDistance();
};

#endif