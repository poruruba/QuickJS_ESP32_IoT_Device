const BMP280_ADDRESS = 0x76; /**< The default I2C address for the sensor. */
const BMP280_CHIPID = 0x58; /**< Default chip ID. */

const BMP280_REGISTER_DIG_T1 = 0x88;
const BMP280_REGISTER_DIG_T2 = 0x8A;
const BMP280_REGISTER_DIG_T3 = 0x8C;
const BMP280_REGISTER_DIG_P1 = 0x8E;
const BMP280_REGISTER_DIG_P2 = 0x90;
const BMP280_REGISTER_DIG_P3 = 0x92;
const BMP280_REGISTER_DIG_P4 = 0x94;
const BMP280_REGISTER_DIG_P5 = 0x96;
const BMP280_REGISTER_DIG_P6 = 0x98;
const BMP280_REGISTER_DIG_P7 = 0x9A;
const BMP280_REGISTER_DIG_P8 = 0x9C;
const BMP280_REGISTER_DIG_P9 = 0x9E;
const BMP280_REGISTER_CHIPID = 0xD0;
const MP280_REGISTER_VERSION = 0xD1;
const BMP280_REGISTER_SOFTRESET = 0xE0;
const BMP280_REGISTER_CAL26 = 0xE1; /**< R calibration = 0xE1-0xF0 */
const BMP280_REGISTER_STATUS = 0xF3;
const BMP280_REGISTER_CONTROL = 0xF4;
const BMP280_REGISTER_CONFIG = 0xF5;
const BMP280_REGISTER_PRESSUREDATA = 0xF7;
const BMP280_REGISTER_TEMPDATA = 0xFA;

/** Sleep mode. */
const MODE_SLEEP = 0x00;
/** Forced mode. */
const MODE_FORCED = 0x01;
/** Normal mode. */
const MODE_NORMAL = 0x03;
/** Software reset. */
const MODE_SOFT_RESET_CODE = 0xB6;

/** No over-sampling. */
const SAMPLING_NONE = 0x00;
/** 1x over-sampling. */
const SAMPLING_X1 = 0x01;
/** 2x over-sampling. */
const SAMPLING_X2 = 0x02;
/** 4x over-sampling. */
const SAMPLING_X4 = 0x03;
/** 8x over-sampling. */
const SAMPLING_X8 = 0x04;
/** 16x over-sampling. */
const SAMPLING_X16 = 0x05;

/** No filtering. */
const FILTER_OFF = 0x00;
/** 2x filtering. */
const FILTER_X2 = 0x01;
/** 4x filtering. */
const FILTER_X4 = 0x02;
/** 8x filtering. */
const FILTER_X8 = 0x03;
/** 16x filtering. */
const FILTER_X16 = 0x04;

/** 1 ms standby. */
const STANDBY_MS_1 = 0x00;
/** 62.5 ms standby. */
const STANDBY_MS_63 = 0x01;
/** 125 ms standby. */
const STANDBY_MS_125 = 0x02;
/** 250 ms standby. */
const STANDBY_MS_250 = 0x03;
/** 500 ms standby. */
const STANDBY_MS_500 = 0x04;
/** 1000 ms standby. */
const STANDBY_MS_1000 = 0x05;
/** 2000 ms standby. */
const STANDBY_MS_2000 = 0x06;
/** 4000 ms standby. */
const STANDBY_MS_4000 = 0x07;

export default
class BMP280{
  constructor(wire, address=0x58){
    this.address = address;
    this.wire = wire;
    this._bmp280_calib = {};
    this._measReg = {};
    this._configReg = {};
  }

  async begin(addr = BMP280_ADDRESS, chipid = BMP280_CHIPID){
    this.address = addr;

    var ret = this.read8(BMP280_REGISTER_CHIPID);
    if( ret != chipid )
      throw 'chipid mismatch';

    this.readCoefficients();
    this.setSampling();
    this.setSampling(MODE_NORMAL, SAMPLING_X2, SAMPLING_X16, FILTER_X16, STANDBY_MS_500);
    
    await this.sleep(100);
  }

  readPressure() {
    // Must be done first to get the t_fine variable set up
    this.readTemperature();
  
    var adc_P = this.read24(BMP280_REGISTER_PRESSUREDATA);
    adc_P >>= 4;
  
    var var1 = this.t_fine - 128000;
    var var2 = var1 * var1 * this._bmp280_calib.dig_P6;
    var2 = var2 + (var1 * this._bmp280_calib.dig_P5) * this.pow2(17);
    var2 = var2 + this._bmp280_calib.dig_P4 * this.pow2(35);
    var1 = (var1 * var1 * this._bmp280_calib.dig_P3) * this.pow2(8) + (var1 * this._bmp280_calib.dig_P2) * this.pow2(12);
    var1 = (this.pow2(47) + var1) * this._bmp280_calib.dig_P1 * this.pow2(33);
  
    if (var1 == 0) {
      return 0; // avoid exception caused by division by zero
    }

    var p = 1048576 - adc_P;
    p = Math.floor((((p << 31) - var2) * 3125) / var1);
    var1 = Math.floor((this._bmp280_calib.dig_P9) * Math.floor(p / this.pow2(13)) * Math.floor(p / this.pow2(13))) / this.pow2(25);
    var2 = Math.floor((this._bmp280_calib.dig_P8 * p) / this.pow2(19));
  
    p = Math.floor((p + var1 + var2) / this.pow2(8)) + this._bmp280_calib.dig_P7 * this.pow2(4);
    return p / 256.0;
  }
  
  readTemperature() {
   var adc_T = this.read24(BMP280_REGISTER_TEMPDATA);
   adc_T >>= 4;
 
   var var1 = ((((adc_T >> 3) - (this._bmp280_calib.dig_T1 << 1))) * (this._bmp280_calib.dig_T2)) >> 11; 
   var var2 = (((((adc_T >> 4) - (this._bmp280_calib.dig_T1)) * ((adc_T >> 4) - (this._bmp280_calib.dig_T1))) >> 12) * (this._bmp280_calib.dig_T3)) >> 14;
   this.t_fine = var1 + var2;
   var T = (this.t_fine * 5 + 128) >> 8;

   return T / 100.0;
 }
 
  readCoefficients(){
    this._bmp280_calib.dig_T1 = this.read16_LE(BMP280_REGISTER_DIG_T1);
    this._bmp280_calib.dig_T2 = this.readS16_LE(BMP280_REGISTER_DIG_T2);
    this._bmp280_calib.dig_T3 = this.readS16_LE(BMP280_REGISTER_DIG_T3);
  
    this._bmp280_calib.dig_P1 = this.read16_LE(BMP280_REGISTER_DIG_P1);
    this._bmp280_calib.dig_P2 = this.readS16_LE(BMP280_REGISTER_DIG_P2);
    this._bmp280_calib.dig_P3 = this.readS16_LE(BMP280_REGISTER_DIG_P3);
    this._bmp280_calib.dig_P4 = this.readS16_LE(BMP280_REGISTER_DIG_P4);
    this._bmp280_calib.dig_P5 = this.readS16_LE(BMP280_REGISTER_DIG_P5);
    this._bmp280_calib.dig_P6 = this.readS16_LE(BMP280_REGISTER_DIG_P6);
    this._bmp280_calib.dig_P7 = this.readS16_LE(BMP280_REGISTER_DIG_P7);
    this._bmp280_calib.dig_P8 = this.readS16_LE(BMP280_REGISTER_DIG_P8);
    this._bmp280_calib.dig_P9 = this.readS16_LE(BMP280_REGISTER_DIG_P9);
  }

  setSampling(mode = MODE_NORMAL, tempSampling = SAMPLING_X16, pressSampling = SAMPLING_X16, filter = FILTER_OFF, duration = STANDBY_MS_1){
    this._measReg.mode = mode;
    this._measReg.osrs_t = tempSampling;
    this._measReg.osrs_p = pressSampling;

    this._configReg.filter = filter;
    this._configReg.t_sb = duration;

    this.write8(BMP280_REGISTER_CONFIG, this._configReg_get());
    this.write8(BMP280_REGISTER_CONTROL, this._measReg_get());
  }

  _configReg_get(){
    return (this._configReg.t_sb << 5) | (this._configReg.filter << 2) | this._configReg.spi3w_en;
  }

  _measReg_get(){
    return (this._measReg.osrs_t << 5) | (this._measReg.osrs_p << 2) | this._measReg.mode;
  }

  write8(reg, value) {
    this.wire.beginTransmission(this.address);
    var ret = this.wire.write([reg, value]);
    if( ret != 2 )
      throw 'failed';
    var ret = this.wire.endTransmission();
    if( ret != 0 )
      throw 'failed';
  }
  
  read16_LE(reg) {
    var temp = this.read16(reg);
    return (temp >> 8) | ((temp & 0xff) << 8);
  }
  
  readS16_LE(reg) {
    var ret = this.read16_LE(reg);
    if( ret & 0x8000 )
      return ~ret + 1;
    else
      return ret;
  }

  readS16(reg){
    var ret = this.read16(reg);
    if( ret & 0x8000 )
      return ~ret + 1;
    else
      return ret;
  }

  read8(reg) {
    this.wire.beginTransmission(this.address);
    var ret = this.wire.write(reg);
    if( ret != 1 )
      throw 'failed';
    var ret = this.wire.endTransmission();
    if( ret != 0 )
      throw 'failed';
    var ret = this.wire.requestFrom(this.address, 1);
    if( ret != 1 )
      throw 'failed';

    var value = this.wire.read();

    return value;
  }

  read16(reg) {
    this.wire.beginTransmission(this.address);
    var ret = this.wire.write(reg);
    if( ret != 1 )
      throw 'failed';
    var ret = this.wire.endTransmission();
    if( ret != 0 )
      throw 'failed';
    var ret = this.wire.requestFrom(this.address, 2);
    if( ret != 2 )
      throw 'failed';
    var ret = this.wire.read(2);
    var value = (ret[0] << 8) | ret[1];
  
    return value;
  }
  
  read24(reg) {
    this.wire.beginTransmission(this.address);
    var ret = this.wire.write(reg);
    if( ret != 1 )
      throw 'failed';
    var ret = this.wire.endTransmission();
    if( ret != 0 )
      throw 'failed';
    var ret = this.wire.requestFrom(this.address, 3);
    if( ret != 3 )
      throw 'failed';
    var ret = this.wire.read(3);

    var value = (ret[0] << 16) | (ret[1] << 8) | ret[2]
  
    return value;
  }
  
  async sleep(msec){
  	return new Promise(resolve => setTimeout(resolve, msec));
  }

  pow2(n){
    var val = 1;
    for( var i = 0 ; i < n ; i++ )
      val *= 2;
    
    return val;
  }
}

//module.exports = BMP280;
