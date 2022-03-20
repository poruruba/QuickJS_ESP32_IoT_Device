import * as gpio from "Gpio";
import * as input from "Input";

var led = false;

function setup(){
	gpio.pinMode(10, gpio.OUTPUT);
	gpio.digitalWrite(10, led ? gpio.LOW : gpio.HIGH);
	console.log('setup finished');
}

function loop(){
	esp32.update();

	if( input.wasPressed(input.BUTTON_A) ){
		console.log('Pressed');
		led = !led;
		gpio.digitalWrite(10, led ? gpio.LOW : gpio.HIGH);
	}
}
