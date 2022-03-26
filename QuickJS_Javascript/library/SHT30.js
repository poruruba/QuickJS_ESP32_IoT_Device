export default
class SHT30 {
  constructor(wire, address = 0x44) {
    this.wire = wire;
    this._address = address;
    this.cTemp = 0;
    this.fTemp = 0;
    this.humidity = 0;
  }

  async get() {
    // Start I2C Transmission
    this.wire.beginTransmission(this._address);
    // Send measurement command
    this.wire.write(0x2C);
    this.wire.write(0x06);
    // Stop I2C transmission
    if (this.wire.endTransmission() != 0)
      return 1;

	await this.sleep(500);

    // Request 6 bytes of data
    this.wire.requestFrom(this._address, 6);

    // Read 6 bytes of data
    // cTemp msb, cTemp lsb, cTemp crc, humidity msb, humidity lsb, humidity crc
    var data = this.wire.read(6);

    await this.sleep(50);

    if (this.wire.available() != 0)
      return 2;

    // Convert the data
    this.cTemp = ((((data[0] * 256.0) + data[1]) * 175) / 65535.0) - 45;
    this.fTemp = (this.cTemp * 1.8) + 32;
    this.humidity = ((((data[3] * 256.0) + data[4]) * 100) / 65535.0);

    return 0;
  }
  
  async sleep(msec){
  	return new Promise(resolve => setTimeout(resolve, msec));
  }
}

//module.exports = SHT30;
