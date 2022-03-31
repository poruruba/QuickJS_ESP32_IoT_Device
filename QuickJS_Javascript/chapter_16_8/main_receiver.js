import * as ir from "Ir";

ir.recvBegin(33);
ir.recvStart();

setInterval(() =>{
	var value = ir.checkRecv(1);
	if( value )
		console.log(value);
}, 1);
