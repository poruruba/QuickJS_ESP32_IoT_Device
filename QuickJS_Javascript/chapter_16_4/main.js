import * as lcd from "Lcd";
import * as imu from "Imu";

function setup(){
	lcd.setFont(16);
	lcd.setTextSize(1);
	lcd.fillScreen(0x000000);
}

async function sleep(msec){
	return new Promise(resolve => setTimeout(resolve, msec) );
}

async function loop(){
	var accl = imu.getAccelData();
	var gyro = imu.getGyroData();

	lcd.fillScreen(0x000000);
	lcd.setCursor(0, 0);
	lcd.println("accl: x=" + accl.x.toFixed(2) + " y=" + accl.y.toFixed(2) + " z=" + accl.z.toFixed(2));
	lcd.println("gyro: x=" + gyro.x.toFixed(2) + " y=" + gyro.y.toFixed(2) + " z=" + gyro.z.toFixed(2));
	
	await sleep(100);
}
