const Arduino = require('./Arduino.js');

var arduino = new Arduino("http://192.168.1.245");
const interval = 1000;

(async () =>{
await arduino.console.log("setup finished");

    setInterval( async () =>{
    await arduino.console.log('Hello World');
      }, interval);
})();
