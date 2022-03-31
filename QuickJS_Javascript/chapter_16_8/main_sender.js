import * as ir from "Ir";
import * as input from "Input";

ir.sendBegin(9);

var counter = 0;

setInterval(() =>{
	esp32.update();

	if( input.wasPressed(input.BUTTON_A) ){
		counter++;
		ir.send(counter);
	}
}, 1);
