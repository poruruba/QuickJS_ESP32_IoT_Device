'use strict';

class Prefs{
  constructor(arduino){
    this.arduino = arduino;
    this.module_type = "/prefs-";
  }

  async clear(name){
    var params = {
      name: name,
    };
    return this.arduino.webapi_request(this.module_type + "clear", params);
  }

  async remove(name, key){
    var params = {
      name: name,
      key: key,
    };
    return this.arduino.webapi_request(this.module_type + "remove", params);
  }

  async isKey(name, key){
    var params = {
      name: name,
      key: key,
    };
    return this.arduino.webapi_request(this.module_type + "isKey", params);
  }

  async getType(name, key){
    var params = {
      name: name,
      key: key,
    };
    return this.arduino.webapi_request(this.module_type + "getType", params);
  }

  async putNumber(name, key, value){
    var params = {
      name: name,
      key: key,
      value: value
    };
    return this.arduino.webapi_request(this.module_type + "putNumber", params);
  }

  async getNumber(name, key, def){
    var params = {
      name: name,
      key: key,
      def: def
    };
    return this.arduino.webapi_request(this.module_type + "getNumber", params);
  }
  
  async putDouble(name, key, value){
    var params = {
      name: name,
      key: key,
      value: value
    };
    return this.arduino.webapi_request(this.module_type + "putDouble", params);
  }

  async getDouble(name, key, def){
    var params = {
      name: name,
      key: key,
      def: def
    };
    return this.arduino.webapi_request(this.module_type + "getDouble", params);
  }

  async putBool(name, key, value){
    var params = {
      name: name,
      key: key,
      value: value
    };
    return this.arduino.webapi_request(this.module_type + "putBool", params);
  }

  async getBool(name, key, def){
    var params = {
      name: name,
      key: key,
      def: def
    };
    return this.arduino.webapi_request(this.module_type + "getBool", params);
  }

  async putString(name, key, value){
    var params = {
      name: name,
      key: key,
      value: value
    };
    return this.arduino.webapi_request(this.module_type + "putString", params);
  }

  async getString(name, key){
    var params = {
      name: name,
      key: key,
    };
    return this.arduino.webapi_request(this.module_type + "getString", params);
  }

  async putBytes(name, key, value){
    var params = {
      name: name,
      key: key,
      value: his.arduino.bufferToBase64(value)
    };
    return this.arduino.webapi_request(this.module_type + "putBytes", params);
  }

  async getBytes(name, key, def){
    var params = {
      name: name,
      key: key,
      def: def
    };
    var b64 = this.arduino.webapi_request(this.module_type + "getBytes", params);
    return this.arduino.base64ToBuffer(b64);
  }
}

//module.exports = Prefs;
