#include "SONIC_IO.h"

void SONIC_IO::begin(
  uint8_t trig, 
  uint8_t echo
) {
  _trig = trig;
  _echo = echo;
  pinMode(_trig, OUTPUT); // Sets the trigPin as an Output
  pinMode(_echo, INPUT); // Sets the echoPin as an Input
}

float SONIC_IO::getDuration() {

  digitalWrite(_trig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trig, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  float duration = pulseIn(_echo, HIGH);
  
  return duration;
}

float SONIC_IO::getDistance() {
  // Calculating the distance
  float Distance = getDuration()*0.34/2;
  if(Distance > 4500.00){
    return 4500.00;
  }else{
    return Distance;
  }
}
