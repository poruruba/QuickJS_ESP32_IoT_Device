const Arduino = require('./Arduino.js');
const Gpio = require('./driver/Gpio.js');
const LedBlink = require('./LedBlink.js');

(async () =>{
	var arduino = new Arduino("http://192.168.1.215");
	var gpio = new Gpio(arduino);
	var ledblink = new LedBlink(gpio, 10);
	var interval = 1000;
	await ledblink.start(interval);
	
	setInterval(async () =>{
		interval -= 100;
		if( interval <= 0 )
			interval = 1000;
		await ledblink.start(interval);
	}, 4000);
})();
