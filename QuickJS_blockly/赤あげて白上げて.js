import * as ledc from 'Ledc';
import * as input from 'Input';
import * as lcd from 'Lcd';
import * as gpio from "Gpio";

var max = 8100, min = 1700;
var center = 4600;
var freq = [(max - center) / 3 + center, center, (min - center) / 3 + center];
var is_running = false;
var counter;
var num_of_ok;
var num_of_attack;
var num_of_win = 0;

lcd.setRotation(0);
lcd.fillScreen(0x000000);
lcd.setTextSize(2);
lcd.setCursor(0, 0);
lcd.print("Ready?");

gpio.pinMode(32, gpio.INPUT);
gpio.pinMode(33, gpio.INPUT);

ledc.setup(0, 50, 16);
ledc.attachPin(26, 0);

input.onButtonWasPressed(input.BUTTON_A, () => {
	if( is_running )
		return;
		
	num_of_ok = 0;
	num_of_attack = 5;
	counter = num_of_attack ;

	lcd.fillScreen(0x000000);
	lcd.setCursor(0, 0);
	lcd.print("GO!");

	is_running = true;
});

async function loop(){
	if( !is_running )
		return;

	ledc.write(0, center);
	await wait_async(1000);

	ledc.write(0, freq[random(freq.length)]);
	await wait_async(random(200, 800) / (num_of_win + 1));

	ledc.write(0, freq[random(freq.length)]);
	await wait_async(random(200, 800) / (num_of_win + 1));

	var answer = random(2);
	ledc.write(0, answer ? max : min);

	lcd.fillScreen(0x000000);
	lcd.setCursor(0, 0);
	lcd.println("赤?青?");
	var ready_answer = true;
	var start = millis();
	while( (millis() - start) < 2000 / (num_of_win + 1) ){
		if( !ready_answer )
			continue;

		var btn1 = gpio.digitalRead(32);
		var btn2 = gpio.digitalRead(33);

		if( btn1 != 0 && btn2 == 0 ){
			if( answer == 1 ){
				lcd.print("OK");
				num_of_ok++;
			}else{
				lcd.print("NG");
			}
			ready_answer = false;
		}else if( btn1 == 0 && btn2 != 0 ){
			if( answer == 0 ){
				lcd.print("OK");
				num_of_ok++;
			}else{
				lcd.print("NG");
			}
			ready_answer = false;
		}
	}
	if( ready_answer  ){
		lcd.print("NG");
		ready_answer = false;
	}

	counter--;
	if( counter <= 0 ){
		if( num_of_ok >= num_of_attack )
			num_of_win++;

		lcd.fillScreen(0x000000);
		lcd.setCursor(0, 0);

		lcd.println("試合結果");
		lcd.println(num_of_ok + ' / ' + num_of_attack);
		lcd.println("完勝数");
		lcd.println(num_of_win);
		
		is_running  = false;
	}
}
