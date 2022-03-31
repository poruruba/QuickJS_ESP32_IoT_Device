import * as input from "Input";
import * as udp from "Udp";

var counter = 0;
var host = 0xC0A801FF;
var port = 3333;

setInterval(() =>{
	esp32.update();

	if( input.wasPressed(input.BUTTON_A) ){
		counter++;
		udp.sendText(host, port, "counter=" + counter);
	}
}, 1);
