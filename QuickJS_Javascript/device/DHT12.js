export default
class DHT12{
  constructor(wire, scale = 1, id = 0x5c){
    this.CELSIUS = 1;
    this.KELVIN = 2;
    this.FAHRENHEIT = 3;

    this.wire = wire;
    this.scale = scale;
    this.address = id;
  }

  async read(){
    this.wire.beginTransmission(this.address);
    var ret = this.wire.write(0);
    if( ret != 1 )
      throw 'failed';
    var ret = this.wire.endTransmission();
    if( ret != 0 )
      throw 'failed';

    var ret = this.wire.requestFrom(this.address, 5);
    if( ret != 5 )
      throw 'failed';

    var datos = this.wire.read(5);
    
    this.sleep(50);

    var ret = this.wire.available();
    if( ret != 0 )
      throw 'failed';

    if (datos[4] != (datos[0] + datos[1] + datos[2] + datos[3]) )
      throw 'datos error';

    this.datos = datos;
  }

  async readTemperature(scale){
    await this.read();
    if( scale == undefined )
      scale = this.scale;

    var resultado = 0.0;
  	switch(scale) {
      case this.CELSIUS:
        resultado = this.datos[2] + this.datos[3] / 10.0;
        break;
      case this.FAHRENHEIT:
        resultado = (this.datos[2] + this.datos[3] / 10.0) * 1.8 + 32.0;
        break;
      case this.KELVIN:
        resultado = (this.datos[2] + this.datos[3] / 10.0) + 273.15;
        break;
  	};
  	return resultado;
  }

  async readHumidity(){
    await this.read();

    var resultado = (this.datos[0] + this.datos[1] / 10.0);
    return resultado;
  }

  async sleep(msec){
    return new Promise(resolve => setTimeout(resolve, msec));
  }
}

//module.exports = DHT12;
