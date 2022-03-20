const Arduino = require('./Arduino.js');
const Gpio = require('./driver/Gpio.js');

(async () =>{
	var arduino = new Arduino("http://192.168.1.245");
	var gpio = new Gpio(arduino);
	var led = false;

	await gpio.pinMode(10, gpio.OUTPUT);
	await gpio.digitalWrite(10, led ? gpio.LOW : gpio.HIGH);

	setInterval(async () =>{
		console.log("pressed");
		await arduino.console.log('pressed');
		led = !led;
		await gpio.digitalWrite(10, led ? gpio.LOW : gpio.HIGH);
	}, 1000);
})();
