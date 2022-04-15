import * as gpio from "Gpio";
import * as ledc from "Ledc";

ledc.setup(0, 50, 16);
ledc.attachPin(26, 0);

gpio.pinMode(32, gpio.INPUT);

const appear = 4600;
const hide = 8100;
var counter = hide;

ledc.write(0, counter);

async function loop(){
	if( gpio.analogRead(33) < 500 ){
		counter = hide;
		ledc.write(0, counter);
	}else{
		if( counter > appear ){
			counter -= 50;
			ledc.write(0, counter);
		}
	}
	await wait_async(50);
}