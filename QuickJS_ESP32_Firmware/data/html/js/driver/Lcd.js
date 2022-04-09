'use strict';

class Lcd{
  constructor(arduino){
   this.arduino = arduino;
   this.module_type = "/lcd-";
    
   this.top_left = 0;
   this.top_center = 1;
   this.top_right = 2;
   this.middle_left = 4;
   this.middle_center = 5;
   this.middle_right = 6;
   this.bottom_left = 8;
   this.bottom_center = 9;
   this.bottom_right = 10;
   this.baseline_left = 16;
   this.baseline_center = 17;
   this.baseline_right = 18;
  }

  async setRotation(rot){
    var params = {
        rot: rot
    };
    return this.arduino.webapi_request(this.module_type + "setRotation", params);
  }

  async setBrightness(brt){
    var params = {
        brt: brt
    };
    return this.arduino.webapi_request(this.module_type + "setBrightness", params);
  }

  async setFont(size){
    var params = {
      size: size
    };
    return this.arduino.webapi_request(this.module_type + "setFont", params);
  }

  async setTextColor(fore, back){
    var params = {
      fore: fore
    };
    if( back !== undefined )
      params.back = back;
    return this.arduino.webapi_request(this.module_type + "setTextColor", params);
  }

  async setTextDatum(datum){
    var params = {
      datum: datum
    };
    return this.arduino.webapi_request(this.module_type + "setTextDatum", params);
  }

  async drawPixel(x, y, color){
    var params = {
      x: x,
      y: y,
      color: color
    };
    return this.arduino.webapi_request(this.module_type + "drawPixel", params);
  }

  async drawLine(x0, y0, x1, y1, color){
    var params = {
      x0: x0,
      y0: y0,
      x1: x1,
      y1: y1
    };
    if( color !== undefined )
      params.color = color;
    return this.arduino.webapi_request(this.module_type + "drawLine", params);
  }

  async drawRect(x, y, w, h, color){
    var params = {
      x: x,
      y: y,
      w: w,
      h: h
    };
    if( color !== undefined )
      params.color = color;
    return this.arduino.webapi_request(this.module_type + "drawRect", params);
  }

  async fillRect(x, y, w, h, color){
    var params = {
      x: x,
      y: y,
      w: w,
      h: h
    };
    if( color !== undefined )
      params.color = color;
    return this.arduino.webapi_request(this.module_type + "fillRect", params);
  }

  async drawRoundRect(x, y, w, h, r, color){
    var params = {
      x: x,
      y: y,
      w: w,
      h: h,
      r: r
    };
    if( color !== undefined )
      params.color = color;
    return this.arduino.webapi_request(this.module_type + "drawRoundRect", params);
  }

  async fillRoundRect(x, y, w, h, r, color){
    var params = {
      x: x,
      y: y,
      w: w,
      h: h,
      r: r
    };
    if( color !== undefined )
      params.color = color;
    return this.arduino.webapi_request(this.module_type + "fillRoundRect", params);
  }
  
  async drawCircle(x, y, r, color){
    var params = {
      x: x,
      y: y,
      r: r
    };
    if( color !== undefined )
      params.color = color;
    return this.arduino.webapi_request(this.module_type + "drawCircle", params);
  }

  async fillCircle(x, y, r, color){
    var params = {
      x: x,
      y: y,
      r: r
    };
    if( color !== undefined )
      params.color = color;
    return this.arduino.webapi_request(this.module_type + "fillCircle", params);
  }
  
  async setCursor(x, y){
    var params = {
      x: x,
      y: y,
    };
    return this.arduino.webapi_request(this.module_type + "setCursor", params);
  }
  
  async getCursor(){
    var params = {
    };
    return this.arduino.webapi_request(this.module_type + "getCursor", params);
  }

  async textWidth(text){
    var params = {
      text: text
    };
    return this.arduino.webapi_request(this.module_type + "textWidth", params);
  }

  async print(message){
    var params = {
      message: message
    };
    return this.arduino.webapi_request(this.module_type + "print", params);
  }

  async println(message){
    var params = {
      message: message
    };
    return this.arduino.webapi_request(this.module_type + "println", params);
  }

  async fillScreen(color){
    var params = {
      color: color
    };
    return this.arduino.webapi_request(this.module_type + "fillScreen", params);
  }

  async drawImageFile(filename, x, y){
    var params = {
      filename: filename
    };
    if( x !== undefined && y != undefined ){
      params.x = x;
      params.y = y;
    }
    return this.arduino.webapi_request(this.module_type + "drawImageFile", params);
  }

  async width(){
    var params = {
    };
    return this.arduino.webapi_request(this.module_type + "width", params);
  }

  async height(){
    var params = {
    };
    return this.arduino.webapi_request(this.module_type + "height", params);
  }

  async getColorDepth(){
    var params = {
    };
    return this.arduino.webapi_request(this.module_type + "getColorDepth", params);
  }

  async fontHeight(){
    var params = {
    };
    return this.arduino.webapi_request(this.module_type + "fontHeight", params);
  }
}

//module.exports = Lcd;
