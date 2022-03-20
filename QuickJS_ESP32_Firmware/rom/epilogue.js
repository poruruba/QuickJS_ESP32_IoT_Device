

var setup_finished = false;
if (typeof setup === 'function'){
  async function call_setup(){
    try{
      await setup();
    }catch(error){
      console.log(error);
    }finally{
      setup_finished = true;
    }
  }
  call_setup();
}else{
  setup_finished = true;
}

var loop_running = false;
if (typeof loop === 'function'){
  async function check_and_call_loop(){
    if ( !setup_finished )
      return;
    if ( !loop_running ){
      loop_running = true;
      try{
        await loop();
      }catch(error){
        console.log(error);
        delay(1000);
      }finally{
        loop_running = false;
      }
    }
  }
  esp32.setLoop(check_and_call_loop);
}