import * as pixels from "Pixels";
import * as input from "Input";

const PIXES_PIN = 27;
const NUM_OF_LED_WIDTH = 5;
const NUM_OF_LED_HEIGHT = 5;
const LED_BACK_COLOR = 0x000000;

var counter = 0;

const NumberMatric = [
	[
		0b01110,
		0b10011,
		0b10101,
		0b11001,
		0b01110,
	],
	[
		0b00100,
		0b01100,
		0b00100,
		0b00100,
		0b01110,
	],
	[
		0b11110,
		0b00001,
		0b01110,
		0b10100,
		0b11111,
	],
	[
		0b11110,
		0b00001,
		0b00110,
		0b10001,
		0b11110,
	],
	[
		0b10001,
		0b10001,
		0b01111,
		0b00001,
		0b00001,
	],
	[
		0b11111,
		0b10000,
		0b11110,
		0b00001,
		0b11110,
	],
	[
		0b01111,
		0b10000,
		0b11110,
		0b10001,
		0b01110,
	],
	[
		0b11111,
		0b00001,
		0b00010,
		0b00010,
		0b00010,
	],
	[
		0b01110,
		0b10001,
		0b01110,
		0b10001,
		0b01110,
	],
	[
		0b01110,
		0b10001,
		0b01111,
		0b00001,
		0b01110,
	]
];

function setup(){
	pixels.begin(PIXES_PIN, NUM_OF_LED_WIDTH * NUM_OF_LED_HEIGHT);
	for( var i = 0 ; i < NUM_OF_LED_WIDTH * NUM_OF_LED_HEIGHT ; i++ )
		pixels. setPixelColor(i, LED_BACK_COLOR);
	pixels.setOnoff(true);
	
	drawNumber(counter);
}

function drawNumber(no){
	var index = 0;
	var color = Math.floor(Math.random(0x1000000) * 0x1000000);
	for( var i = 0 ; i < NUM_OF_LED_HEIGHT ; i++ ){
		for( var j = 0 ; j < NUM_OF_LED_WIDTH ; j++ ){
			var b = (NumberMatric[no][i] >> (NUM_OF_LED_WIDTH - j - 1)) & 0x01;
			pixels.setPixelColor(index++,  b ? color : LED_BACK_COLOR);
		}
	}
}

async function loop(){
	esp32.update();

	if( input.wasPressed(input.BUTTON_A) ){
		counter++;
		if( counter >= NumberMatric.length )
			counter = 0;
		drawNumber(counter);
	}
}
