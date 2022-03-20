import * as wire from "Wire";
//import SHT30 from "SHT30.js";
import DHT12 from "DHT12.js";
import * as rtc from "Rtc";
import * as sd from "Sd";

const fname = "/envdata.csv";

// var sht30;
var dht12;

//const SDA = 0;
//const SCL = 26;
const SDA = 32;
const SCL = 33;

function setup(){
	wire.begin(SDA, SCL);
//	sht30 = new SHT30(wire);
	dht12 = new DHT12(wire);
	
	sd.writeText(fname, "");
}

async function sleep(msec){
	return new Promise(resolve => setTimeout(resolve, msec) );
}

async function loop(){
	esp32.update();

// for SHT30
//	var ret = await sht30.get();
//	if( ret == 0 ){
//		console.log(sht30.cTemp.toFixed(2) + "," + sht30.humidity.toFixed(2) + getDateTimeString() + "\n" );
//		sd.appendText(fname, sht30.cTemp.toFixed(2) + "," + sht30.humidity.toFixed(2) + getDateTimeString() + "\n");
//	}

// for DHT12
	var temp = await dht12.readTemperature();
	var humidity = await dht12.readHumidity();
	console.log(temp.toFixed(2) + "," + humidity.toFixed(2) + "," + getDateTimeString() + "\n" );
	sd.appendText(fname, temp.toFixed(2) + "," + humidity.toFixed(2) + "," + getDateTimeString() + "\n");

	await sleep(60000);
}

function getDateTimeString(){
	var date = rtc.getDate();
	var time = rtc.getTime();
	
	return `${date.Year}/${date.Month}/${date.Date} ${time.Hours}:${time.Minutes}:${time.Seconds}`;
}
