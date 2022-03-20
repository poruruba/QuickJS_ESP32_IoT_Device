import * as lcd from "Lcd";
import * as rtc from "Rtc";
import * as input from "Input";
import * as utils from "Utils";

var counter = 0;

function setup(){
	lcd.setFont(16);
	lcd.setTextSize(1);
	lcd.fillScreen(0x000000);
}

async function loop(){
	esp32.update();

	var text = esp32.checkPutText();
	if( text ){
		var date = rtc.getDate();
		var time = rtc.getTime();
		lcd.setCursor(0, 0);
		lcd.fillScreen(0x000000);
		lcd.println(date.Year + "/" + date.Month + "/" + date.Date);
		lcd.println(time.Hours + ":" + time.Minutes + ":" + time.Seconds);
		lcd.println("Received");
		lcd.println(text);
	}
	
	if( input.wasPressed(input.BUTTON_A) ){
		counter++;
		var params = {
			endpoint: "/putText",
			params: {
				text: "counter=" + counter
			}
		};
		utils.httpPostJson("http://192.168.1.245/endpoint", params );
	}
}
