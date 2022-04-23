import * as input from "Input";
import Test from "TestModule.js";

var test = new Test();
console.log('setup finished');

setInterval(() =>{
	esp32.update();
	if( input.wasPressed(input.BUTTON_A) ){
		test.fire();
	}
}, 1);
