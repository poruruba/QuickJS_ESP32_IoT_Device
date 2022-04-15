import * as gpio from "Gpio";
import * as ledc from "Ledc";
import * as pixels from "Pixels";

pixels.begin(32, 14);
pixels.clear();
ledc.setup(0, 50, 16);
ledc.attachPin(26, 0);

const appear = 4600;
const hide = 8100;

async function loop(){
	var pos = gpio.analogRead(33);
	ledc.write(0, pos / 4096 * (hide - appear) + appear);
	if( pos > 4000 ){
		for( var i = 0 ; i < 14 ; i++ )
			pixels.setPixelColor(i, 0x00ff00);
	}else{
		pixels.clear();
	}
	await wait_async(50);
}