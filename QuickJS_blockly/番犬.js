import * as gpio from "Gpio";
import * as ledc from "Ledc";

const appear = 4600;
const hide = 8100;
var counter = hide;

ledc.setup(0, 50, 16);
ledc.attachPin(26, 0);
ledc.write(0, counter);

gpio.pinMode(33, gpio.INPUT);

async function loop(){
	if( gpio.digitalRead(33) ){
		counter = appear + random(-100, 100);
		ledc.write(0, counter);
	}else{
		if( counter < hide ){
			counter += 100;
			ledc.write(0, counter);
		}
	}
	await wait_async(50);
}