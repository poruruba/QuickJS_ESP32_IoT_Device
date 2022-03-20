var last_tim = 0;
const interval = 1000;

function setup(){
	console.log('setup finished');
}

function loop(){
	var now = millis();
	if( now >= last_tim + interval ){
                console.log('Hello World');
		last_tim = now;
	}
}
