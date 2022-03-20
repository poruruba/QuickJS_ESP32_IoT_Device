'use strict';

class Sd{
  constructor(arduino){
   this.arduino = arduino;
   this.module_type = "/sd-";
  }

  async exists(filename){
    var params = {
      filename: filename
    };
    return this.arduino.webapi_request(this.module_type + "exists", params);
  }

  async mkdir(filename){
    var params = {
      filename: filename
    };
    return this.arduino.webapi_request(this.module_type + "mkdir", params);
  }

  async remove(filename){
    var params = {
      filename: filename
    };
    return this.arduino.webapi_request(this.module_type + "remove", params);
  }

  async rmdir(filename){
    var params = {
      filename: filename
    };
    return this.arduino.webapi_request(this.module_type + "rmdir", params);
  }

  async size(filename){
    var params = {
      filename: filename
    };
    return this.arduino.webapi_request(this.module_type + "size", params);
  }
  
  async readBinary(filename, offset, size){
    var params = {
      filename: filename,
      offset: offset,
      size: size
    };
    var b64 = await this.arduino.webapi_request(this.module_type + "readBinary", params);
    return Buffer.from(b64, 'base64').buffer;
  }

  async writeBinary(filename, buffer, offset, size){
    var params = {
      filename: filename,
      buffer: Buffer.from(buffer).toString('base64'),
      offset: offset,
      size: size
    };
    return this.arduino.webapi_request(this.module_type + "writeBinary", params);
  }

  async readText(filename){
    var params = {
      filename: filename,
    };
    return this.arduino.webapi_request(this.module_type + "readText", params);
  }

  async writeText(filename, text){
    var params = {
      filename: filename,
      text: text,
    };
    return this.arduino.webapi_request(this.module_type + "writeText", params);
  }

  async appendText(filename, text){
    var params = {
      filename: filename,
      text: text,
    };
    return this.arduino.webapi_request(this.module_type + "appendText", params);
  }

  async isDirectory(filename){
    var params = {
      filename: filename
    };
    return this.arduino.webapi_request(this.module_type + "isDirectory", params);
  }  

  async list(filename){
    var params = {
      filename: filename
    };
    return this.arduino.webapi_request(this.module_type + "list", params);
  }
}

module.exports = Sd;
