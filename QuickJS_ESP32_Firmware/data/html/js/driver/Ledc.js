'use strict';

class Ledc{
  constructor(arduino){
   this.arduino = arduino;
   this.module_type = "/ledc-";
    
   this.NOTE_C = 0;
   this.NOTE_Cs = 1;
   this.NOTE_D = 2;
   this.NOTE_Eb = 3;
   this.NOTE_E = 4;
   this.NOTE_F = 5;
   this.NOTE_Fs = 6;
   this.NOTE_G = 7;
   this.NOTE_Gs = 8;
   this.NOTE_A = 9;
   this.NOTE_Bb = 10;
   this.NOTE_B = 11;
   this.NOTE_MAX = 12;
  }

  async setup(channel, freq, resolution){
    var params = {
      channel: channel,
      freq: freq,
      resolution: resolution
    };
    return this.arduino.webapi_request(this.module_type + "setup", params);
  }

  async attachPin(pin, channel){
    var params = {
      pin: pin,
      channel: channel,
    };
    return this.arduino.webapi_request(this.module_type + "attachPin", params);
  }

  async detachPin(pin){
    var params = {
      pin: pin,
    };
    return this.arduino.webapi_request(this.module_type + "detachPin", params);
  }

  async write(channel, duty){
    var params = {
      channel: channel,
      duty: duty,
    };
    return this.arduino.webapi_request(this.module_type + "write", params);
  }

  async writeTone(channel, freq){
    var params = {
      channel: channel,
      freq: freq,
    };
    return this.arduino.webapi_request(this.module_type + "writeTone", params);
  }

  async writeNote(channel, note, octave){
    var params = {
      channel: channel,
      note: note,
      octave: octave
    };
    return this.arduino.webapi_request(this.module_type + "writeNote", params);
  }

  async read(channel){
    var params = {
      channel: channel,
    };
    return this.arduino.webapi_request(this.module_type + "read", params);
  }

  async readFreq(channel){
    var params = {
      channel: channel,
    };
    return this.arduino.webapi_request(this.module_type + "readFreq", params);
  }
}

//module.exports = Ledc;
