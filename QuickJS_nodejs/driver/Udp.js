'use strict';

const dgram = require('dgram');

class Udp{
  constructor(){
    this.udpSocket = dgram.createSocket('udp4');
  }

  send(host, port, payload){
    this.udpSocket.send(payload, port, host);
  }
}

module.exports = Udp;