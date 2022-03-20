'use strict';

class Pixels{
  constructor(arduino){
   this.arduino = arduino;
   this.module_type = "/pixels-";
  }

  async begin(pin){
    var params = {
      pin: pin,
    };
    return this.arduino.webapi_request(this.module_type + "begin", params);
  }

  async clear(){
    return this.arduino.webapi_request(this.module_type + "clear", {});
  }

  async setOnoff(onoff){
    var params = {
      onoff: onoff
    };
    return this.arduino.webapi_request(this.module_type + "setOnoff", params);
  }

  async getOnoff(){
    return this.arduino.webapi_request(this.module_type + "getOnoff", {});
  }
  
  async setPixelColor(index, color){
    var params = {
      index: index,
      color: color,
    };
    return this.arduino.webapi_request(this.module_type + "setPixelColor", params);
  }

  async getPixelColor(index){
    var params = {
      index: index,
    };
    return this.arduino.webapi_request(this.module_type + "getPixelColor", params);
  }
}

module.exports = Pixels;
