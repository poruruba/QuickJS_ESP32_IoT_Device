'use strict';

class Gpio{
  constructor(arduino){
   this.arduino = arduino;
   this.module_type = "/gpio-";
    
   this.LOW = 0x0;
   this.HIGH = 0x1;

   this.INPUT = 0x01;
   this.OUTPUT = 0x02;
   this.PULLUP = 0x04;
   this.INPUT_PULLUP = 0x05;
   this.PULLDOWN = 0x08;
   this.INPUT_PULLDOWN = 0x09;
   this.OPEN_DRAIN = 0x10;
   this.OUTPUT_OPEN_DRAIN = 0x12;
  }

  async pinMode(pin, mode){
    var params = {
      pin: pin,
      mode: mode
    };
    return this.arduino.webapi_request(this.module_type + "pinMode", params);
  }

  async digitalWrite(pin, val){
    var params = {
      pin: pin,
      value: val,
    };
    return this.arduino.webapi_request(this.module_type + "digitalWrite", params);
  }

  async digitalRead(pin){
    var params = {
      pin: pin,
    };
    return this.arduino.webapi_request(this.module_type + "digitalRead", params);
  }

  async analogRead(pin){
    var params = {
      pin: pin,
    };
    return this.arduino.webapi_request(this.module_type + "analogRead", params);
  }
}

//module.exports = Gpio;
