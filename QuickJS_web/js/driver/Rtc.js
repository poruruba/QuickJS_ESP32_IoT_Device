'use strict';

class Rtc{
  constructor(arduino){
   this.arduino = arduino;
   this.module_type = "/rtc-";
  }

  async setTime(Hours, Minutes, Seconds){
    var params = {
      Hours: Hours,
      Minutes: Minutes,
      Seconds: Seconds
    };
    return this.arduino.webapi_request(this.module_type + "setTime", params);
  }

  async getTime(){
    return this.arduino.webapi_request(this.module_type + "getTime", {});
  }
  
  async setDate(Years, Month, Date, WeekDay){
    var params = {
      Years: Years,
      Month: Month,
      Date: Date,
      WeekDay: WeekDay
    };
    return this.arduino.webapi_request(this.module_type + "setDate", params);
  }

  async getDate(){
    return this.arduino.webapi_request(this.module_type + "getDate", {});
  }
}

//module.exports = Rtc;
