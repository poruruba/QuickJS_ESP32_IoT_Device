export default
class MEGA328 {
  constructor(wire, address = 0x61) {
    this.wire = wire;
    this._address = address;
  }

  async hub_a_read_value(reg){
    await this.wire.beginTransmission(this._address);
    await this.wire.write(reg | 0x06);
    await this.wire.endTransmission();
    
    var RegValue_L,RegValue_H;

    await this.wire.requestFrom(this._address,2);
    while(await this.wire.available()){
      RegValue_L = await this.wire.read();
      RegValue_H = await this.wire.read();
    }
    
    return (RegValue_H << 8) | RegValue_L;   
  } 

  async hub_d_read_value_A(reg){
    await this.wire.beginTransmission(this._address);
    await this.wire.write(reg | 0x04);
    await this.wire.endTransmission();
  
    var RegValue;

    await this.wire.requestFrom(this._address, 1);
    while(await this.wire.available()){
      RegValue = await this.wire.read();
    }
    return RegValue;   
  } 

  async hub_d_read_value_B(reg){
    await this.wire.beginTransmission(this._address);
    await this.wire.write(reg | 0x05);
    await this.wire.endTransmission();
    
    var RegValue;

    await this.wire.requestFrom(this._address, 1);
    while(await this.wire.available()){
      RegValue = await this.wire.read();
    }
    return RegValue;   
  } 

  async hub_d_wire_value_A(reg, level){
    await this.wire.beginTransmission(this._address);
    await this.wire.write(reg | 0x00);
    await this.wire.write(level & 0xff);
    await this.wire.endTransmission(); 
  } 

  async hub_d_wire_value_B(reg, level){
    await this.wire.beginTransmission(this._address);
    await this.wire.write(reg | 0x01);
    await this.wire.write(level & 0xff);
    await this.wire.endTransmission(); 
  } 

  async hub_a_wire_value_A(reg, duty){
    await this.wire.beginTransmission(this._address);
    await this.wire.write(reg | 0x02);
    await this.wire.write(duty & 0xff);
    await this.wire.endTransmission(); 
  } 

  async hub_a_wire_value_B(reg, duty){
    await this.wire.beginTransmission(this._address);
    await this.wire.write(reg | 0x03);
    await this.wire.write(duty & 0xff);
    await this.wire.endTransmission(); 
  } 

  async hub_wire_length(reg, length){
    await this.wire.beginTransmission(this._address);
    await this.wire.write(reg | 0x08);
    await this.wire.write(length & 0xff);
    await this.wire.write(length>>8);
    await this.wire.endTransmission(); 
  } 

  async hub_wire_index_color(reg, num, r, g, b){
    await this.wire.beginTransmission(this._address);
    await this.wire.write(reg | 0x09);
    await this.wire.write(num & 0xff);
    await this.wire.write(num>>8);
    await this.wire.write(r);
    await this.wire.write(g);
    await this.wire.write(b);
    await this.wire.endTransmission(); 
  } 

  async hub_wire_fill_color(reg, first, count, r, g, b){
    await this.wire.beginTransmission(this._address);
    await this.wire.write(reg | 0x0a);
    await this.wire.write(first & 0xff);
    await this.wire.write(first>>8);

    await this.wire.write(count & 0xff);
    await this.wire.write(count>>8);

    await this.wire.write(r);
    await this.wire.write(g);
    await this.wire.write(b);
    await this.wire.endTransmission(); 
  }

  async hub_wire_setBrightness(reg, brightness){
    await this.wire.beginTransmission(this._address);
    await this.wire.write(reg | 0x0b);
    await this.wire.write(brightness & 0xff);
    await this.wire.endTransmission(); 
  }
}

//module.exports = MEGA328;