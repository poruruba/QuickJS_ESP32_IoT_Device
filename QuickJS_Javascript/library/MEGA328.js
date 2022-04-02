export default
class MEGA328 {
  constructor(wire, address = 0x61) {
    this.wire = wire;
    this._address = address;
  }

  hub_a_read_value(reg){
    this.wire.beginTransmission(this._address);
    this.wire.write(reg | 0x06);
    this.wire.endTransmission();
    
    var RegValue_L,RegValue_H;

    this.wire.requestFrom(this._address,2);
    while( this.wire.available()){
      RegValue_L =  this.wire.read();
      RegValue_H =  this.wire.read();
    }
    
    return (RegValue_H << 8) | RegValue_L;   
  } 

  hub_d_read_value_A(reg){
    this.wire.beginTransmission(this._address);
    this.wire.write(reg | 0x04);
    this.wire.endTransmission();
 
    var RegValue;

    this.wire.requestFrom(this._address, 1);
    while( this.wire.available()){
      RegValue =  this.wire.read();
    }
    return RegValue;   
  }

   hub_d_read_value_B(reg){
     this.wire.beginTransmission(this._address);
     this.wire.write(reg | 0x05);
     this.wire.endTransmission();
    
    var RegValue;

     this.wire.requestFrom(this._address, 1);
    while( this.wire.available()){
      RegValue =  this.wire.read();
    }
    return RegValue;   
  } 

   hub_d_wire_value_A(reg, level){
     this.wire.beginTransmission(this._address);
     this.wire.write(reg | 0x00);
     this.wire.write(level & 0xff);
     this.wire.endTransmission(); 
  } 

   hub_d_wire_value_B(reg, level){
     this.wire.beginTransmission(this._address);
     this.wire.write(reg | 0x01);
     this.wire.write(level & 0xff);
     this.wire.endTransmission(); 
  } 

   hub_a_wire_value_A(reg, duty){
     this.wire.beginTransmission(this._address);
     this.wire.write(reg | 0x02);
     this.wire.write(duty & 0xff);
     this.wire.endTransmission(); 
  } 

   hub_a_wire_value_B(reg, duty){
     this.wire.beginTransmission(this._address);
     this.wire.write(reg | 0x03);
     this.wire.write(duty & 0xff);
     this.wire.endTransmission(); 
  } 

   hub_wire_length(reg, length){
     this.wire.beginTransmission(this._address);
     this.wire.write(reg | 0x08);
     this.wire.write(length & 0xff);
     this.wire.write(length>>8);
     this.wire.endTransmission(); 
  } 

   hub_wire_index_color(reg, num, r, g, b){
     this.wire.beginTransmission(this._address);
     this.wire.write(reg | 0x09);
     this.wire.write(num & 0xff);
     this.wire.write(num>>8);
     this.wire.write(r);
     this.wire.write(g);
     this.wire.write(b);
     this.wire.endTransmission(); 
  } 

   hub_wire_fill_color(reg, first, count, r, g, b){
     this.wire.beginTransmission(this._address);
     this.wire.write(reg | 0x0a);
     this.wire.write(first & 0xff);
     this.wire.write(first>>8);

     this.wire.write(count & 0xff);
     this.wire.write(count>>8);

     this.wire.write(r);
     this.wire.write(g);
     this.wire.write(b);
     this.wire.endTransmission(); 
  }

   hub_wire_setBrightness(reg, brightness){
     this.wire.beginTransmission(this._address);
     this.wire.write(reg | 0x0b);
     this.wire.write(brightness & 0xff);
     this.wire.endTransmission(); 
  }
}

//module.exports = MEGA328;