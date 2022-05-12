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
  var dropdown_async = block.getFieldValue('async');
  var value_msec = Blockly.JavaScript.valueToCode(block, 'msec', Blockly.JavaScript.ORDER_ATOMIC);
  var statements_func = Blockly.JavaScript.statementToCode(block, 'func');
  // TODO: Assemble JavaScript into code variable.
  var code = 'setTimeout(' + (dropdown_async==='true' ? ' async ' : "") + '() => {\n' + statements_func + '}, ' + value_msec + ');\n';
  return code;
};

Blockly.JavaScript['setinterval'] = function(block) {
  var dropdown_async = block.getFieldValue('async');
  var value_msec = Blockly.JavaScript.valueToCode(block, 'msec', Blockly.JavaScript.ORDER_ATOMIC);
  var statements_func = Blockly.JavaScript.statementToCode(block, 'func');
  // TODO: Assemble JavaScript into code variable.
  var code = 'setInterval(' + (dropdown_async==='true' ? ' async ' : "" ) + '() => {\n' + statements_func + '}, ' + value_msec + ');\n';
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
  var code = "'" + text_key + "': " + value_value;
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_ATOMIC];
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
    for( var i = 0 ; i < list.length ; i++ ){
      if( list[i] )
      code += "import * as " + list[i].toLowerCase() + " from '" + list[i] + "';\n";
  }
  }
  return code;
};

Blockly.JavaScript['program_module'] = function(block) {
  var dropdown_module = block.getFieldValue('module');
  // TODO: Assemble JavaScript into code variable.
  var code = '"' + dropdown_module + '"';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_ATOMIC];
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
  var dropdown_btn = block.getFieldValue('btn');
  // TODO: Assemble JavaScript into code variable.
  var code = 'input.isPressed(' + dropdown_btn + ')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['input_onbuttonwaspressed'] = function(block) {
  var dropdown_async = block.getFieldValue('async');
  var dropdown_btn = block.getFieldValue('btn');
  var statements_func = Blockly.JavaScript.statementToCode(block, 'func');
  // TODO: Assemble JavaScript into code variable.
  var code = 'input.onButtonWasPressed(' + dropdown_btn + ', ' + (dropdown_async==='true' ? "async " : "") + '() => {\n' + statements_func + '});\n';
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


Blockly.JavaScript['udp_recvbegin'] = function(block) {
  var value_port = Blockly.JavaScript.valueToCode(block, 'port', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'udp.recvBegin(' + value_port + ');\n';
  return code;
};

Blockly.JavaScript['udp_sendtext'] = function(block) {
  var value_host = Blockly.JavaScript.valueToCode(block, 'host', Blockly.JavaScript.ORDER_ATOMIC);
  var value_port = Blockly.JavaScript.valueToCode(block, 'port', Blockly.JavaScript.ORDER_ATOMIC);
  var value_text = Blockly.JavaScript.valueToCode(block, 'text', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'udp.sendText(' + value_host + ', ' + value_port + ', ' + value_text + ');\n';
  return code;
};

Blockly.JavaScript['udp_recvstop'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'udp.recvStop();\n';
  return code;
};

Blockly.JavaScript['udp_checkrecv'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'udp.checkRecv()';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['lcd_setrotation'] = function(block) {
  var value_rot = Blockly.JavaScript.valueToCode(block, 'rot', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.setRotation(' + value_rot + ');\n';
  return code;
};

Blockly.JavaScript['lcd_setbrightness'] = function(block) {
  var value_brt = Blockly.JavaScript.valueToCode(block, 'brt', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.setBrightness(' + value_brt + ');\n';
  return code;
};

Blockly.JavaScript['lcd_setfont'] = function(block) {
  var value_size = Blockly.JavaScript.valueToCode(block, 'size', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.setFont(' + value_size + ');\n';
  return code;
};

Blockly.JavaScript['lcd_settextcolor'] = function(block) {
  var value_fore = Blockly.JavaScript.valueToCode(block, 'fore', Blockly.JavaScript.ORDER_ATOMIC);
  var value_back = Blockly.JavaScript.valueToCode(block, 'back', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.setTextColor(' + value_fore + ', ' + value_back + ');\n';
  return code;
};

Blockly.JavaScript['lcd_settextsize'] = function(block) {
  var value_scale = Blockly.JavaScript.valueToCode(block, 'scale', Blockly.JavaScript.ORDER_ATOMIC);
  var value_yscale = Blockly.JavaScript.valueToCode(block, 'yscale', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.setTextSize(' + value_scale + ', ' + value_yscale + ');\n';
  return code;
};

Blockly.JavaScript['lcd_settextdatum'] = function(block) {
  var dropdown_datum = block.getFieldValue('datum');
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.setTextDatum(' + dropdown_datum + ');\n';
  return code;
};

Blockly.JavaScript['lcd_drawpixel'] = function(block) {
  var value_x = Blockly.JavaScript.valueToCode(block, 'x', Blockly.JavaScript.ORDER_ATOMIC);
  var value_y = Blockly.JavaScript.valueToCode(block, 'y', Blockly.JavaScript.ORDER_ATOMIC);
  var value_color = Blockly.JavaScript.valueToCode(block, 'color', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.drawPixel(' + value_x + ', ' + value_y + ', ' + value_color + ');\n';
  return code;
};

Blockly.JavaScript['lcd_drawline'] = function(block) {
  var value_x0 = Blockly.JavaScript.valueToCode(block, 'x0', Blockly.JavaScript.ORDER_ATOMIC);
  var value_y0 = Blockly.JavaScript.valueToCode(block, 'y0', Blockly.JavaScript.ORDER_ATOMIC);
  var value_x1 = Blockly.JavaScript.valueToCode(block, 'x1', Blockly.JavaScript.ORDER_ATOMIC);
  var value_y1 = Blockly.JavaScript.valueToCode(block, 'y1', Blockly.JavaScript.ORDER_ATOMIC);
  var value_color = Blockly.JavaScript.valueToCode(block, 'color', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.drawLine(' + value_x0 + ', ' + value_y0 + ', ' + value_x1 + ', ' + value_y1 + ', ' + value_color + ');\n';
  return code;
};

Blockly.JavaScript['lcd_drawrect'] = function(block) {
  var value_x = Blockly.JavaScript.valueToCode(block, 'x', Blockly.JavaScript.ORDER_ATOMIC);
  var value_y = Blockly.JavaScript.valueToCode(block, 'y', Blockly.JavaScript.ORDER_ATOMIC);
  var value_w = Blockly.JavaScript.valueToCode(block, 'w', Blockly.JavaScript.ORDER_ATOMIC);
  var value_h = Blockly.JavaScript.valueToCode(block, 'h', Blockly.JavaScript.ORDER_ATOMIC);
  var value_color = Blockly.JavaScript.valueToCode(block, 'color', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.drawRect(' + value_x + ', ' + value_y + ', ' + value_w + ', ' + value_h + ', ' + value_color + ');\n';
  return code;
};

Blockly.JavaScript['lcd_fillrect'] = function(block) {
  var value_x = Blockly.JavaScript.valueToCode(block, 'x', Blockly.JavaScript.ORDER_ATOMIC);
  var value_y = Blockly.JavaScript.valueToCode(block, 'y', Blockly.JavaScript.ORDER_ATOMIC);
  var value_w = Blockly.JavaScript.valueToCode(block, 'w', Blockly.JavaScript.ORDER_ATOMIC);
  var value_h = Blockly.JavaScript.valueToCode(block, 'h', Blockly.JavaScript.ORDER_ATOMIC);
  var value_color = Blockly.JavaScript.valueToCode(block, 'color', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.fillRect(' + value_x + ', ' + value_y + ', ' + value_w + ', ' + value_h + ', ' + value_color +  ');\n';
  return code;
};

Blockly.JavaScript['lcd_fillroundrect'] = function(block) {
  var value_x = Blockly.JavaScript.valueToCode(block, 'x', Blockly.JavaScript.ORDER_ATOMIC);
  var value_y = Blockly.JavaScript.valueToCode(block, 'y', Blockly.JavaScript.ORDER_ATOMIC);
  var value_w = Blockly.JavaScript.valueToCode(block, 'w', Blockly.JavaScript.ORDER_ATOMIC);
  var value_h = Blockly.JavaScript.valueToCode(block, 'h', Blockly.JavaScript.ORDER_ATOMIC);
  var value_r = Blockly.JavaScript.valueToCode(block, 'r', Blockly.JavaScript.ORDER_ATOMIC);
  var value_color = Blockly.JavaScript.valueToCode(block, 'color', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.fillRoundRect(' + value_x + ', ' + value_y + ', ' + value_w + ', '+ value_h + ', ' + value_r + ', ' + value_color + ');\n';
  return code;
};

Blockly.JavaScript['lcd_drawroundrect'] = function(block) {
  var value_x = Blockly.JavaScript.valueToCode(block, 'x', Blockly.JavaScript.ORDER_ATOMIC);
  var value_y = Blockly.JavaScript.valueToCode(block, 'y', Blockly.JavaScript.ORDER_ATOMIC);
  var value_w = Blockly.JavaScript.valueToCode(block, 'w', Blockly.JavaScript.ORDER_ATOMIC);
  var value_h = Blockly.JavaScript.valueToCode(block, 'h', Blockly.JavaScript.ORDER_ATOMIC);
  var value_color = Blockly.JavaScript.valueToCode(block, 'color', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.drawRoundRect(' + value_x + ', ' + value_y + ', ' + value_w + ', '+ value_h + ', ' + value_r + ', ' + value_color + ');\n';
  return code;
};

Blockly.JavaScript['lcd_drawcircle'] = function(block) {
  var value_x = Blockly.JavaScript.valueToCode(block, 'x', Blockly.JavaScript.ORDER_ATOMIC);
  var value_y = Blockly.JavaScript.valueToCode(block, 'y', Blockly.JavaScript.ORDER_ATOMIC);
  var value_r = Blockly.JavaScript.valueToCode(block, 'r', Blockly.JavaScript.ORDER_ATOMIC);
  var value_color = Blockly.JavaScript.valueToCode(block, 'color', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.drawCircle(' + value_x + ', ' + value_y + ', ' + value_r + ', ' + value_color + ');\n';
  return code;
};

Blockly.JavaScript['lcd_fillcircle'] = function(block) {
  var value_x = Blockly.JavaScript.valueToCode(block, 'x', Blockly.JavaScript.ORDER_ATOMIC);
  var value_y = Blockly.JavaScript.valueToCode(block, 'y', Blockly.JavaScript.ORDER_ATOMIC);
  var value_r = Blockly.JavaScript.valueToCode(block, 'r', Blockly.JavaScript.ORDER_ATOMIC);
  var value_color = Blockly.JavaScript.valueToCode(block, 'color', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.fillCircle(' + value_x + ', ' + value_y + ', ' + value_r + ', ' + value_color + ');\n';
  return code;
};

Blockly.JavaScript['lcd_setcursor'] = function(block) {
  var value_x = Blockly.JavaScript.valueToCode(block, 'x', Blockly.JavaScript.ORDER_ATOMIC);
  var value_y = Blockly.JavaScript.valueToCode(block, 'y', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.setCursor(' + value_x + ', ' + value_y + ');\n';
  return code;
};

Blockly.JavaScript['lcd_getcursorx'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.getCursor().x';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['lcd_getcursory'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.getCursor().y';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['lcd_textwidth'] = function(block) {
  var value_text = Blockly.JavaScript.valueToCode(block, 'text', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.textWidth(' + value_text + ')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['lcd_print'] = function(block) {
  var value_message = Blockly.JavaScript.valueToCode(block, 'message', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.print(' + value_message + ');\n';
  return code;
};

Blockly.JavaScript['lcd_println'] = function(block) {
  var value_message = Blockly.JavaScript.valueToCode(block, 'message', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.println(' + value_message + ');\n';
  return code;
};

Blockly.JavaScript['lcd_fillscreen'] = function(block) {
  var value_color = Blockly.JavaScript.valueToCode(block, 'color', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.fillScreen(' + value_color + ');\n';
  return code;
};

Blockly.JavaScript['lcd_drawimagefile'] = function(block) {
  var value_filename = Blockly.JavaScript.valueToCode(block, 'filename', Blockly.JavaScript.ORDER_ATOMIC);
  var value_x = Blockly.JavaScript.valueToCode(block, 'x', Blockly.JavaScript.ORDER_ATOMIC);
  var value_y = Blockly.JavaScript.valueToCode(block, 'y', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.drawImageFile(' + value_filename + ', ' + value_x + ', ' + value_y + ');\n';
  return code;
};

Blockly.JavaScript['lcd_width'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.width()';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['lcd_height'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.height()';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['lcd_getcolordepth'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.getColorDepth()';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['lcd_fontheight'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'lcd.fontHeight()';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['ledc_setup'] = function(block) {
  var value_channel = Blockly.JavaScript.valueToCode(block, 'channel', Blockly.JavaScript.ORDER_ATOMIC);
  var value_freq = Blockly.JavaScript.valueToCode(block, 'freq', Blockly.JavaScript.ORDER_ATOMIC);
  var value_resolution = Blockly.JavaScript.valueToCode(block, 'resolution', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'ledc.setup(' + value_channel + ', ' + value_freq + ', ' + value_resolution + ');\n';
  return code;
};

Blockly.JavaScript['ledc_attachpin'] = function(block) {
  var value_pin = Blockly.JavaScript.valueToCode(block, 'pin', Blockly.JavaScript.ORDER_ATOMIC);
  var value_channel = Blockly.JavaScript.valueToCode(block, 'channel', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'ledc.attachPin(' + value_pin + ', ' + value_channel + ');\n';
  return code;
};

Blockly.JavaScript['ledc_detachpin'] = function(block) {
  var value_pin = Blockly.JavaScript.valueToCode(block, 'pin', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'ledc.detachPin(' + value_pin + ');\n';
  return code;
};

Blockly.JavaScript['ledc_write'] = function(block) {
  var value_channel = Blockly.JavaScript.valueToCode(block, 'channel', Blockly.JavaScript.ORDER_ATOMIC);
  var value_duty = Blockly.JavaScript.valueToCode(block, 'duty', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'ledc.write(' + value_channel + ', ' + value_duty + ');\n';
  return code;
};

Blockly.JavaScript['ledc_writetone'] = function(block) {
  var value_channel = Blockly.JavaScript.valueToCode(block, 'channel', Blockly.JavaScript.ORDER_ATOMIC);
  var value_freq = Blockly.JavaScript.valueToCode(block, 'freq', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'ledc.writeTone(' + value_channel + ', ' + value_freq + ');\n';
  return code;
};

Blockly.JavaScript['ledc_writenote'] = function(block) {
  var value_channel = Blockly.JavaScript.valueToCode(block, 'channel', Blockly.JavaScript.ORDER_ATOMIC);
  var dropdown_note = block.getFieldValue('note');
  var value_octave = Blockly.JavaScript.valueToCode(block, 'octave', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'ledc.writeNote(' + value_channel + ', ' + dropdown_note + ', ' + value_octave + ');\n';
  return code;
};

Blockly.JavaScript['ledc_read'] = function(block) {
  var value_channel = Blockly.JavaScript.valueToCode(block, 'channel', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'ledc.read(' + value_channel + ')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['ledc_readfreq'] = function(block) {
  var value_channel = Blockly.JavaScript.valueToCode(block, 'channel', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'ledc.readFreq(' + value_channel + ')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['ir_sendbegin'] = function(block) {
  var value_pin = Blockly.JavaScript.valueToCode(block, 'pin', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'ir.sendBegin(' + value_pin + ');\n';
  return code;
};

Blockly.JavaScript['ir_send'] = function(block) {
  var value_data = Blockly.JavaScript.valueToCode(block, 'data', Blockly.JavaScript.ORDER_ATOMIC);
  var value_repeat = Blockly.JavaScript.valueToCode(block, 'repeat', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'ir.send(' + value_data + ', ' + value_repeat + ');\n';
  return code;
};

Blockly.JavaScript['ir_recvbegin'] = function(block) {
  var value_pin = Blockly.JavaScript.valueToCode(block, 'pin', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'ir.recvBegin(' + value_pin + ');\n';
  return code;
};

Blockly.JavaScript['ir_recvstart'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'ir.recvStart();\n';
  return code;
};

Blockly.JavaScript['ir_recvstop'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'ir.recvStop();\n';
  return code;
};

Blockly.JavaScript['ir_checkrecv'] = function(block) {
  var dropdown_type = block.getFieldValue('type');
  // TODO: Assemble JavaScript into code variable.
  var code = 'ir.checkRecv(' + dropdown_type + ')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['mqtt_connect'] = function(block) {
  var value_client_name = Blockly.JavaScript.valueToCode(block, 'client_name', Blockly.JavaScript.ORDER_ATOMIC);
  var value_buffer_size = Blockly.JavaScript.valueToCode(block, 'buffer_size', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'mqtt.connect(' + value_client_name + ', ' + value_buffer_size + ');\n';
  return code;
};

Blockly.JavaScript['mqtt_connect_user_pass_'] = function(block) {
  var value_client_name = Blockly.JavaScript.valueToCode(block, 'client_name', Blockly.JavaScript.ORDER_ATOMIC);
  var value_buffer_size = Blockly.JavaScript.valueToCode(block, 'buffer_size', Blockly.JavaScript.ORDER_ATOMIC);
  var value_username = Blockly.JavaScript.valueToCode(block, 'username', Blockly.JavaScript.ORDER_ATOMIC);
  var value_password = Blockly.JavaScript.valueToCode(block, 'password', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'mqtt.connect(' + value_client_name + ', ' + value_buffer_size + ', ' + value_username + ', ' + value_password + ');\n';
  return code;
};

Blockly.JavaScript['mqtt_disconnect'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'mqtt.disconnect();\n';
  return code;
};

Blockly.JavaScript['mqtt_subscribe'] = function(block) {
  var dropdown_async = block.getFieldValue('async');
  var value_topic = Blockly.JavaScript.valueToCode(block, 'topic', Blockly.JavaScript.ORDER_ATOMIC);
  var statements_func = Blockly.JavaScript.statementToCode(block, 'func');
  // TODO: Assemble JavaScript into code variable.
  var code = 'mqtt.subscribe(' + value_topic + ', ' + (dropdown_async==='true' ? "async " : "") + '() => {\n' + statements_func + '});\n';
  return code;
};

Blockly.JavaScript['mqtt_unsubscribe'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'mqtt.unsubscribe();\n';
  return code;
};

Blockly.JavaScript['mqtt_publish'] = function(block) {
  var value_topic = Blockly.JavaScript.valueToCode(block, 'topic', Blockly.JavaScript.ORDER_ATOMIC);
  var value_payload = Blockly.JavaScript.valueToCode(block, 'payload', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'mqtt.publish(' + value_topic + ', ' + value_payload + ');\n';
  return code;
};

Blockly.JavaScript['mqtt_getpayload'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'mqtt.getPayload().payload';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['mqtt_setserver'] = function(block) {
  var value_host = Blockly.JavaScript.valueToCode(block, 'host', Blockly.JavaScript.ORDER_ATOMIC);
  var value_port = Blockly.JavaScript.valueToCode(block, 'port', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'mqtt.setServer(' + value_host + ', ' + value_port + ');\n';
  return code;
};

Blockly.JavaScript['audio_begin'] = function(block) {
  var dropdown_mode = block.getFieldValue('mode');
  // TODO: Assemble JavaScript into code variable.
  var code = 'audio.begin(' + dropdown_mode + ');\n';
  return code;
};

Blockly.JavaScript['audio_setpinout'] = function(block) {
  var value_bclk = Blockly.JavaScript.valueToCode(block, 'bclk', Blockly.JavaScript.ORDER_ATOMIC);
  var value_lrck = Blockly.JavaScript.valueToCode(block, 'lrck', Blockly.JavaScript.ORDER_ATOMIC);
  var value_dout = Blockly.JavaScript.valueToCode(block, 'dout', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'audio.setPinout(' + value_bclk + ', ' + value_lrck + ', ' + value_dout + ');\n';
  return code;
};

Blockly.JavaScript['audio_update'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'audio.update();\n';
  return code;
};

Blockly.JavaScript['audio_playsd'] = function(block) {
  var value_path = Blockly.JavaScript.valueToCode(block, 'path', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'audio.playSd(' + value_path + ');\n';
  return code;
};

Blockly.JavaScript['audio_setgain'] = function(block) {
  var value_gain = Blockly.JavaScript.valueToCode(block, 'gain', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'audio.setGain(' + value_gain + ');\n';
  return code;
};

Blockly.JavaScript['audio_getgain'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'audio.getGain()';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['audio_isrunning'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'audio.isRunning()';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['audio_stop'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'audio.stop();\n';
  return code;
};

Blockly.JavaScript['reboot'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'esp32.reboot();\n';
  return code;
};

Blockly.JavaScript['restart'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'esp32.restart();\n';
  return code;
};

Blockly.JavaScript['update'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'esp32.update();\n';
  return code;
};

Blockly.JavaScript['loop'] = function(block) {
  var dropdown_async = block.getFieldValue('async');
  var statements_func = Blockly.JavaScript.statementToCode(block, 'func');
  // TODO: Assemble JavaScript into code variable.
  var code = (dropdown_async==='true' ? "async " : "") + 'function loop(){\n' + statements_func + '}\n';
  return code;
};

Blockly.JavaScript['wait_async'] = function(block) {
  var dropdown_await = block.getFieldValue('await');
  var value_msec = Blockly.JavaScript.valueToCode(block, 'msec', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = (dropdown_await==='true' ? "await " : "") + 'wait_async(' + value_msec + ');\n';
  return code;
};

Blockly.JavaScript['env_dht12_readtemperature'] = function(block) {
  var value_fixed = Blockly.JavaScript.valueToCode(block, 'fixed', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'parseFloat(env.dht12_readTemperature().toFixed(' + value_fixed + '))';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['env_dht12_readhumidity'] = function(block) {
  var value_fixed = Blockly.JavaScript.valueToCode(block, 'fixed', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'parseFloat(env.dht12_readHumidity().toFixed(' + value_fixed + '))';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['env_sht30_gettemperature'] = function(block) {
  var value_fixed = Blockly.JavaScript.valueToCode(block, 'fixed', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'parseFloat(env.sht30_get().cTemp.toFixed(' + value_fixed + '))';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['env_sht30_gethumidity'] = function(block) {
  var value_fixed = Blockly.JavaScript.valueToCode(block, 'fixed', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'parseFloat(env.sht30_get().humidity.toFixed(' + value_fixed + '))';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['void'] = function(block) {
  var value_value = Blockly.JavaScript.valueToCode(block, 'value', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = value_value + ';\n';
  return code;
};

Blockly.JavaScript['wire_begin'] = function(block) {
  var value_sda = Blockly.JavaScript.valueToCode(block, 'sda', Blockly.JavaScript.ORDER_ATOMIC);
  var value_scl = Blockly.JavaScript.valueToCode(block, 'scl', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'wire.begin(' + value_sda + ', ' + value_scl + ')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['wire_requestfrom'] = function(block) {
  var value_address = Blockly.JavaScript.valueToCode(block, 'address', Blockly.JavaScript.ORDER_ATOMIC);
  var value_count = Blockly.JavaScript.valueToCode(block, 'count', Blockly.JavaScript.ORDER_ATOMIC);
  var value_stop = Blockly.JavaScript.valueToCode(block, 'stop', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'wire.requestFrom(' + value_address + ', ' + value_count + ', ' + value_stop +')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['wire_begintransaction'] = function(block) {
  var value_address = Blockly.JavaScript.valueToCode(block, 'address', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'wire.beginTransaction(' + value_address + ');\n';
  return code;
};

Blockly.JavaScript['wire_endtransaction'] = function(block) {
  var value_sendstop = Blockly.JavaScript.valueToCode(block, 'sendStop', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'wire.endTransaction(' + value_sendstop + ')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['wire_write'] = function(block) {
  var value_value = Blockly.JavaScript.valueToCode(block, 'value', Blockly.JavaScript.ORDER_ATOMIC);
  // TODO: Assemble JavaScript into code variable.
  var code = 'wire.write(' + value_value + ')';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['wire_available'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'wire.available()';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['wire_read'] = function(block) {
  // TODO: Assemble JavaScript into code variable.
  var code = 'wire.read()';
  // TODO: Change ORDER_NONE to the correct strength.
  return [code, Blockly.JavaScript.ORDER_FUNCTION_CALL];
};