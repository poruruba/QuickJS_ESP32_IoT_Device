import * as audio from "Audio";

audio.begin(audio.INTERNAL_DAC);
audio.playSd("/g_07.mp3");
//audio.playUrl('http://192.168.1.16:20080/g_07.mp3');

setInterval(() =>{
	audio.update();
}, 1);
