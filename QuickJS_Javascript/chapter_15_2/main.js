import * as gpio from "Gpio";
import LedBlink from "LedBlink.js"

(async () =>{
	var ledblink = new LedBlink(gpio, 10);
	var interval = 1000;
	ledblink.start(interval);
	
	setInterval(async () =>{
		interval -= 100;
		if( interval <= 0 )
			interval = 1000;
		ledblink.start(interval);
	}, 4000);
})();
