Blockly.JavaScript['delay'] = function(block) {
  var value_msec = Blockly.JavaScript.valueToCode(block, 'msec', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'delay(' + value_msec + ');\n';
  return code;
};

Blockly.JavaScript['millis'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'millis()';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['settimeout'] = function(block) {
  var value_name = Blockly.JavaScript.valueToCode(block, 'msec', Blockly.JavaScript.ORDER_ATOMIC);
  var statements_name = Blockly.JavaScript.statementToCode(block, 'func');
  // TODO: Assemble JavaScript into code variable.
  var code = 'setTimeout( () => {\n' + statements_name + '}, ' + value_name + ');\n';
  return code;
};

Blockly.JavaScript['setinterval'] = function(block) {
  var value_name = Blockly.JavaScript.valueToCode(block, 'msec', Blockly.JavaScript.ORDER_ATOMIC);
  var statements_name = Blockly.JavaScript.statementToCode(block, 'func');
  // TODO: Assemble JavaScript into code variable.
  var code = 'setInterval( () => {\n' + statements_name + '}, ' + value_name + ');\n';
  return code;
};

Blockly.JavaScript['cleartimeout'] = function(block) {
  var value_id = Blockly.JavaScript.valueToCode(block, 'id', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'clearTimeout(' + value_id + ');\n';
  return code;
};

Blockly.JavaScript['clearinterval'] = function(block) {
  var value_id = Blockly.JavaScript.valueToCode(block, 'id', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'clearInterval(' + value_id + ');\n';
  return code;
};

Blockly.JavaScript['object_field'] = function(block) {
  var text_key = block.getFieldValue('key');
  var value_value = Blockly.JavaScript.valueToCode(block, 'value', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = text_key + ': ' + value_value;
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_NONE];
};

Blockly.JavaScript['object'] = function(block) {
  var value_obj = Blockly.JavaScript.valueToCode(block, 'obj', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = '{ ' + value_obj.slice(1, -1) + ' }';
  // TODO: Change ORDER_NONE to the correct strength.
  return code;
};

Blockly.JavaScript['program_start'] = function(block) {
  var value_modules = Blockly.JavaScript.valueToCode(block, 'modules', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = '';
  if( value_modules ){
    var list = JSON.parse(value_modules);
    for( var i = 0 ; i < list.length ; i++ )
      code += "import * as " + list[i].toLowerCase() + " from '" + list[i] + "';\n";
  }
  return code;
};

Blockly.JavaScript['program_module'] = function(block) {
  var dropdown_module = block.getFieldValue('module');
  // TODO: Assemble JavaScript into code variable.
  var code = '"' + dropdown_module + '"';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_NONE];
};


Blockly.JavaScript['gpio_pinmode'] = function(block) {
  var value_pin = Blockly.JavaScript.valueToCode(block, 'pin', Blockly.JavaScript.ORDER_ATOMIC);
  var dropdown_mode = block.getFieldValue('mode');
  // TODO: Assemble JavaScript into code variable.
  var code = code = 'gpio.pinMode(' + value_pin + ', ' + dropdown_mode + ');\n';
  return code;
};

Blockly.JavaScript['gpio_analogread'] = function(block) {
  var value_pin = Blockly.JavaScript.valueToCode(block, 'pin', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'gpio.analogRead(' + value_pin + ')';
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['gpio_digitalread'] = function(block) {
  var value_pin = Blockly.JavaScript.valueToCode(block, 'pin', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'gpio.digitalRead(' + value_pin + ')';
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['gpio_digitalwrite'] = function(block) {
  var value_pin = Blockly.JavaScript.valueToCode(block, 'pin', Blockly.JavaScript.ORDER_ATOMIC);
  var dropdown_value = block.getFieldValue('value');
  // TODO: Assemble JavaScript into code variable.
  var code = 'gpio.digitalWrite(' + value_pin + ', ' + dropdown_value + ');\n';
  return code;
};

Blockly.JavaScript['input_ispressed'] = function(block) {
  var dropdown_name = block.getFieldValue('btn');
  // TODO: Assemble JavaScript into code variable.
  var code = 'input.isPressed(' + dropdown_name + ')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['input_onbuttonwaspressed'] = function(block) {
  var dropdown_btn = block.getFieldValue('btn');
  var statements_func = Blockly.JavaScript.statementToCode(block, 'func');
  // TODO: Assemble JavaScript into code variable.
  var code = 'input.onButtonWasPressed(' + dropdown_btn + ', () => {\n' + statements_func + '});\n';
  return code;
};

Blockly.JavaScript['pixels_begin'] = function(block) {
  var value_pin = Blockly.JavaScript.valueToCode(block, 'pin', Blockly.JavaScript.ORDER_ATOMIC);
  var value_num = Blockly.JavaScript.valueToCode(block, 'num', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'pixels.begin(' + value_pin + ', ' + value_num + ');\n';
  return code;
};

Blockly.JavaScript['pixels_clear'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'pixels.clear();\n';
  return code;
};

Blockly.JavaScript['pixels_setonoff'] = function(block) {
  var value_onoff = Blockly.JavaScript.valueToCode(block, 'onoff', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'pixels.setOnoff(' + value_onoff + ');\n';
  return code;
};

Blockly.JavaScript['pixels_getonoff'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'pixels.getOnoff()';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['pixels_setpixelcolor'] = function(block) {
  var value_index = Blockly.JavaScript.valueToCode(block, 'index', Blockly.JavaScript.ORDER_ATOMIC);
  var value_color = Blockly.JavaScript.valueToCode(block, 'color', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'pixels.setPixelColor(' + value_index + ', ' + value_color + ');\n';
  return code;
};

Blockly.JavaScript['pixels_getpixelcolor'] = function(block) {
  var value_index = Blockly.JavaScript.valueToCode(block, 'index', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'pixels.getPixelColor()';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['console_log'] = function(block) {
  var value_str = Blockly.JavaScript.valueToCode(block, 'str', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'console.log(' + value_str + ');\n';
  return code;
};

Blockly.JavaScript['utils_rgb2number'] = function(block) {
  var value_rgb = Blockly.JavaScript.valueToCode(block, 'rgb', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'utils.rgb2Number(' + value_rgb + ')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['utils_number2rgb'] = function(block) {
  var value_color = Blockly.JavaScript.valueToCode(block, 'color', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'utils.number2Rgb(' + value_color + ')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};
