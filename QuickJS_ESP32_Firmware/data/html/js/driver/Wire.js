'use strict';

class Wire{
  constructor(arduino, wire = "Wire"){
    this.arduino = arduino;
    if( wire == "Wire1" )
      this.module_type = "/wire1-";
    else
      this.module_type = "/wire-";
  }

  async begin(sda = -1, scl = -1, freq = 0){
    var params = {
      sda: sda,
      scl: scl,
      freq: freq
    };
    return this.arduino.webapi_request(this.module_type + "begin", params);
  }

  async requestFrom(address, count, stop = true){
    var params = {
      address: address,
      count: count,
      stop: stop
    };
    return this.arduino.webapi_request(this.module_type + "requestFrom", params);
  }

  async beginTransmission(address){
    var params = {
      address: address
    };
    return this.arduino.webapi_request(this.module_type + "beginTransmission", params);
  }

  async endTransmission(stop = true){
    var params = {
      stop: stop
    };
    return this.arduino.webapi_request(this.module_type + "endTransmission", params);
  }

  async write(value){
    var params = {
      value: value
    };
    return this.arduino.webapi_request(this.module_type + "write", params);
  }

  async available(){
    return this.arduino.webapi_request(this.module_type + "available", {});
  }

  async read(count){
    if( count == undefined ){
      return this.arduino.webapi_request(this.module_type + "read", {});
    }else{
      var params = {
        count: count
      };
      return this.arduino.webapi_request(this.module_type + "read", params);
    }
  }
}

// module.exports = Wire;