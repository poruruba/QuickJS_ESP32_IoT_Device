import * as gpio from "Gpio";
import Ambient from "Ambient.js";

var channelId = '[チャネルID]';
var writeKey = "[ライトキー]";
const PIR_PIN = 36;

var ambient = new Ambient(channelId, writeKey );
var prev_value  = -1;
gpio.pinMode(PIR_PIN, gpio.INPUT_PULLUP);

setInterval(async () =>{
	var value = gpio.digitalRead(PIR_PIN);
	if( value != prev_value ){
		console.log('triggered');
		prev_value = value;
		await ambient.send({ d1: value } );
	}
}, 5000);
