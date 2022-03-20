import * as lcd from "Lcd";
import * as rtc from "Rtc";

function setup(){
	lcd.setFont(36);
	lcd.setTextSize(2);
}

async function sleep(msec){
	return new Promise(resolve => setTimeout(resolve, msec) );
}

async function loop(){
	var time = rtc.getTime();
	lcd.fillScreen(0x000000);
	lcd.setCursor(0, 0);
	lcd.print(fixed(time.Hours) + ":" + fixed(time.Minutes) );
	await sleep(3000);
}

function fixed(d){
	return ("00" + d).slice(-2);
}
