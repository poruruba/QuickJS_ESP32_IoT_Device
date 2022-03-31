import * as udp from "Udp";

var port = 3333;

udp.recvBegin(port);

setInterval(() =>{
	var text = udp.checkRecvText();
	if( text )
		console.log(text);
}, 1);
