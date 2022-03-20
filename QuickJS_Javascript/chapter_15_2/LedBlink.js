export default
class LedBlink{
  constructor(gpio, pin){
   this.gpio = gpio;
   this.pin = pin;
   this.led = true;
   this.initialized = false;
  }

  start(interval){
    console.log('start(' + interval + ')');
    if( this.interval_id ){
      clearInterval(this.interval_id);
      this.interval_id = null;
    }

    if( !this.initialized ){
      this.gpio.pinMode(this.pin, this.gpio.OUTPUT);
      this.gpio.digitalWrite(this.pin, this.led ? this.gpio.LOW : this.gpio.HIGH);
      this.initialized = true;
    }

    this.interval_id = setInterval( () =>{
      this.led = !this.led;
      this.gpio.digitalWrite(this.pin, this.led ? this.gpio.LOW : this.gpio.HIGH);
    }, interval);
  }
}

//module.exports = LedBlink;
