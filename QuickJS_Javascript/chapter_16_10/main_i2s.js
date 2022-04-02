import * as audio from "Audio";

audio.begin();
//audio.setPinout(12, 0, 2); // for M5Core2
audio.setPinout(19, 33, 22); // for M5Atom Echo
audio.playSd("/g_07.mp3");
//audio.playUrl('http://192.168.1.16:20080/g_07.mp3');

setInterval(() =>{
	audio.update();
}, 1);
