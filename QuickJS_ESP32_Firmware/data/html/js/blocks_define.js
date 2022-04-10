Blockly.Blocks['delay'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("delay");
    this.appendValueInput("msec")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("msec");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['millis'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("millis");
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['settimeout'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("setTimeout");
    this.appendValueInput("msec")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("msec");
    this.appendStatementInput("func")
        .setCheck(null)
        .setAlign(Blockly.ALIGN_RIGHT);
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['cleartimeout'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("clearTimeout");
    this.appendValueInput("id")
        .setCheck("Number")
        .appendField("id");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['setinterval'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("setInterval");
    this.appendValueInput("msec")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("msec");
    this.appendStatementInput("func")
        .setCheck(null)
        .setAlign(Blockly.ALIGN_RIGHT);
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['clearinterval'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("clearInterval");
    this.appendValueInput("id")
        .setCheck("Number")
        .appendField("id");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['console_log'] = {
  init: function() {
    this.appendValueInput("str")
        .appendField("console.log");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['reboot'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("reboot");
    this.setPreviousStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['restart'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("restart");
    this.setPreviousStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['checkputtext'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("checkPutText");
    this.setOutput(true, "Array");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['object_field'] = {
  init: function() {
    this.appendValueInput("value")
        .setCheck(null)
        .appendField("field")
        .appendField(new Blockly.FieldTextInput(""), "key");
    this.setInputsInline(false);
    this.setOutput(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['object'] = {
  init: function() {
    this.appendValueInput("obj")
        .setCheck("Array")
        .appendField("object");
    this.setOutput(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['setsyslogserver'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("setSyslogServer");
    this.appendValueInput("host")
        .setCheck("String")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("topic");
    this.appendValueInput("port")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("port");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['gpio_pinmode'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Gpio.pinMode");
    this.appendValueInput("pin")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("pin");
    this.appendDummyInput()
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("mode")
        .appendField(new Blockly.FieldDropdown([["INPUT","gpio.INPUT"], ["OUTPUT","gpio.OUTPUT"], ["PULLUP","gpio.PULLUP"], ["INPUT_PULLUP","gpio.INPUT_PULLUP"], ["PULLDOWN","gpio.PULLDOWN"], ["INPUT_PULLDOWN","gpio.INPUT_PULLDOWN"], ["OPEN_DRAIN","gpio.OPEN_DRAIN"], ["OUTPUT_OPEN_DRAIN","gpio.OUTPUT_OPEN_DRAIN"]]), "mode");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['gpio_analogread'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Gpio.analogRead");
    this.appendValueInput("pin")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("pin");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['gpio_digitalread'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Gpio.digitalRead");
    this.appendValueInput("pin")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("pin");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['gpio_digitalwrite'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Gpio.digitalWrite");
    this.appendValueInput("pin")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("pin");
    this.appendDummyInput()
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField(new Blockly.FieldDropdown([["LOW","gpio.LOW"], ["HIGH","gpio.HIGH"]]), "value");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['input_ispressed'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Input.isPressed");
    this.appendDummyInput()
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField(new Blockly.FieldDropdown([["BUTTON_A","input.BUTTON_A"], ["BUTTON_B","input.BUTTON_B"], ["BUTTON_C","input.BUTTON_C"]]), "btn");
    this.setInputsInline(true);
    this.setOutput(true, "Boolean");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['input_onbuttonwaspressed'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Input.onButtonWasPressed");
    this.appendDummyInput()
        .appendField(new Blockly.FieldDropdown([["BUTTON_A","input.BUTTON_A"], ["BUTTON_B","input.BUTTON_B"], ["BUTTON_C","input.BUTTON_C"]]), "btn");
    this.appendStatementInput("func")
        .setCheck(null)
        .setAlign(Blockly.ALIGN_RIGHT);
    this.setInputsInline(true);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['pixels_begin'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Pixels.begin");
    this.appendValueInput("pin")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("pin");
    this.appendValueInput("num")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("num");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['pixels_clear'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Pixels.clear");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['pixels_setonoff'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Pixels.setOnoff");
    this.appendValueInput("onoff")
        .setCheck("Boolean")
        .setAlign(Blockly.ALIGN_RIGHT)
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['pixels_getonoff'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Pixels.getOnoff");
    this.setInputsInline(true);
    this.setOutput(true, "Boolean");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['pixels_setpixelcolor'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Pixels.setPixelColor");
    this.appendValueInput("index")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("index");
    this.appendValueInput("color")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("color");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['pixels_getpixelcolor'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Pixels.getPixelColor");
    this.appendValueInput("index")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("index");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['program_start'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Program.Start");
    this.appendValueInput("modules")
        .setCheck("Array")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("modules");
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['program_module'] = {
  init: function() {
    this.appendDummyInput()
        .appendField(new Blockly.FieldDropdown([["Audio","Audio"], ["Gpio","Gpio"], ["Input","Input"], ["Ir","Ir"], ["Lcd","Lcd"], ["Ledc","Ledc"], ["Mqtt","Mqtt"], ["Pixels","Pixels"], ["Udp","Udp"], ["Utils","Utils"]]), "module");
    this.setOutput(true, "String");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['utils_rgb2number'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Utils.rgb2Number");
    this.appendValueInput("rgb")
        .setCheck("Colour")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("rgb");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['utils_number2rgb'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Utils.number2Rgb");
    this.appendValueInput("color")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("color");
    this.setInputsInline(true);
    this.setOutput(true, "Colour");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['udp_recvbegin'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Udp.recvBegin");
    this.appendValueInput("port")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("port");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['udp_sendtext'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Udp.sendText");
    this.appendValueInput("host")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("host");
    this.appendValueInput("port")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("port");
    this.appendValueInput("text")
        .setCheck("String")
        .appendField("text");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['udp_recvstop'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Udp.recvStop");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['udp_checkrecvtext'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Udp.checkRecvText");
    this.setInputsInline(true);
    this.setOutput(true, "String");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_setrotation'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.setRotation");
    this.appendValueInput("rot")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("rot");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_setbrightness'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.setBrightness");
    this.appendValueInput("brt")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("brt");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_setfont'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.setFont");
    this.appendValueInput("size")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("size");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_settextcolor'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.setTextColor");
    this.appendValueInput("fore")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("fore");
    this.appendValueInput("back")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("back");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_settextdatum'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.setDatum");
    this.appendDummyInput()
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("datum")
        .appendField(new Blockly.FieldDropdown([["top_left","lcd.top_left"], ["top_center","lcd.top_center"], ["lcd_right","lcd.lcd_right"], ["middle_left","lcd.middle_left"], ["middle_center","lcd.middle_center"], ["middle_right","lcd.middle_right"], ["bottom_left","lcd.bottom_left"], ["bottom_center","lcd.bottom_center"], ["bottom_right","lcd.bottom_right"], ["baseline_left","lcd.baseline_left"], ["baseline_center","lcd.baseline_center"], ["baseline_right","lcd.baseline_right"]]), "datum");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_drawpixel'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.drawPixel");
    this.appendValueInput("x")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("x");
    this.appendValueInput("y")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.appendValueInput("color")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("color");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_drawline'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.drawLine");
    this.appendValueInput("x0")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("x0");
    this.appendValueInput("y0")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y0");
    this.appendValueInput("x1")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("x1");
    this.appendValueInput("y1")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y1");
    this.appendValueInput("color")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("color");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_drawrect'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.drawRect");
    this.appendValueInput("x")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("x");
    this.appendValueInput("y")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.appendValueInput("w")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("w");
    this.appendValueInput("h")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("h");
    this.appendValueInput("color")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("color");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_fillrect'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.fillRect");
    this.appendValueInput("x")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("x");
    this.appendValueInput("y")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.appendValueInput("w")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("w");
    this.appendValueInput("h")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("h");
    this.appendValueInput("color")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("color");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_fillroundrect'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.fillRoundRect");
    this.appendValueInput("x")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("x");
    this.appendValueInput("y")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.appendValueInput("w")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("w");
    this.appendValueInput("h")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("h");
    this.appendValueInput("r")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("r");
    this.appendValueInput("color")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("color");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_drawroundrect'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.drawRoundRect");
    this.appendValueInput("x")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("x");
    this.appendValueInput("y")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.appendValueInput("w")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("w");
    this.appendValueInput("h")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("h");
    this.appendValueInput("color")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("color");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_drawcircle'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.drawCircle");
    this.appendValueInput("x")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("x");
    this.appendValueInput("y")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.appendValueInput("r")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("r");
    this.appendValueInput("color")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("color");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_fillcircle'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.fillCircle");
    this.appendValueInput("x")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("x");
    this.appendValueInput("y")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.appendValueInput("r")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("r");
    this.appendValueInput("color")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("color");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_setcursor'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.setCursor");
    this.appendValueInput("x")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("x");
    this.appendValueInput("y")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_getcursorx'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.getCursorX");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_getcursory'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.getCursorY");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_textwidth'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.textWidth");
    this.appendValueInput("text")
        .setCheck("String")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("text");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_print'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.print");
    this.appendValueInput("message")
        .setCheck("String")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("message");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_println'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.println");
    this.appendValueInput("message")
        .setCheck("String")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("message");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_fillscreen'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.fillScreen");
    this.appendValueInput("color")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("color");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_drawimagefile'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.drawImageFile");
    this.appendValueInput("filename")
        .setCheck("String")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("filename");
    this.appendValueInput("x")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("x");
    this.appendValueInput("y")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("y");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_width'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.width");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_height'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.height");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_getcolordepth'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.getColorDepth");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['lcd_fontheight'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Lcd.fontHeight");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['ledc_setup'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Ledc.setup");
    this.appendValueInput("channel")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("channel");
    this.appendValueInput("freq")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("freq");
    this.appendValueInput("resolution")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("resolution");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['ledc_attachpin'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Ledc.attachPin");
    this.appendValueInput("pin")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("pin");
    this.appendValueInput("channel")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("channel");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['ledc_detachpin'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Ledc.detachPin");
    this.appendValueInput("pin")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("pin");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['ledc_write'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Ledc.write");
    this.appendValueInput("channel")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("channel");
    this.appendValueInput("pin")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("pin");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['ledc_writetone'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Ledc.writeTone");
    this.appendValueInput("channel")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("channel");
    this.appendValueInput("freq")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("freq");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['ledc_writenote'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Ledc.writeTone");
    this.appendValueInput("channel")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("channel");
    this.appendDummyInput()
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("note")
        .appendField(new Blockly.FieldDropdown([["NOTE_C","ledc.NOTE_C"], ["NOTE_Cs","ledc.NOTE_Cs"], ["NOTE_D","ledc.NOTE_D"], ["NOTE_Eb","ledc.NOTE_Eb"], ["NOTE_E","ledc.NOTE_E"], ["NOTE_F","ledc.NOTE_F"], ["NOTE_Fs","ledc.NOTE_Fs"], ["NOTE_G","ledc.NOTE_G"], ["NOTE_Gs","ledc.NOTE_Gs"], ["NOTE_A","ledc.NOTE_A"], ["NOTE_Bb","ledc.NOTE_Bb"], ["NOTE_B","ledc.NOTE_B"], ["NOTE_MAX","ledc.NOTE_MAX"]]), "note");
    this.appendValueInput("octave")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("octave");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['ledc_read'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Ledc.read");
    this.appendValueInput("channel")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("channel");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['ledc_readfreq'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Ledc.readFreq");
    this.appendValueInput("channel")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("channel");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['ir_sendbegin'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Ir.sendBegin");
    this.appendValueInput("pin")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("pin");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['ir_send'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Ir.send");
    this.appendValueInput("data")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("data");
    this.appendValueInput("repeat")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("repeat");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['ir_recvbegin'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Ir.recvBegin");
    this.appendValueInput("pin")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("pin");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['ir_recvstart'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Ir.recvStart");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['ir_recvstop'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Ir.recvStop");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['ir_checkrecv'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Ir.checkRev");
    this.appendDummyInput()
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("type")
        .appendField(new Blockly.FieldDropdown([["IR_TYPE_NEC","ir.IR_TYPE_NEC"], ["IR_TYPE_SONY","ir.IR_TYPE_SONY"]]), "type");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['mqtt_connect'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Mqtt.connect");
    this.appendValueInput("client_name")
        .setCheck("String")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("client_name");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['mqtt_disconnect'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Mqtt.disconnect");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['mqtt_subscribe'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Mqtt.subscribe");
    this.appendValueInput("topic")
        .setCheck("String")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("topic");
    this.appendStatementInput("func")
        .setCheck(null)
        .setAlign(Blockly.ALIGN_RIGHT)
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['mqtt_unsubscribe'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Mqtt.unsubscribe");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['mqtt_publish'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Mqtt.publish");
    this.appendValueInput("topic")
        .setCheck("String")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("topic");
    this.appendValueInput("payload")
        .setCheck("String")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("payload");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['mqtt_getpayload'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Mqtt.getPayload");
    this.setInputsInline(true);
    this.setOutput(true, "String");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['mqtt_setserver'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Mqtt.setServer");
    this.appendValueInput("host")
        .setCheck("String")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("host");
    this.appendValueInput("port")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("port");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['audio_begin'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Audio.begin");
    this.appendDummyInput()
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("mode")
        .appendField(new Blockly.FieldDropdown([["EXTERNAL_I2S","audio.EXTERNAL_I2S"], ["INTERNAL_DAC","audio.INTERNAL_DAC"], ["INTERNAL_PDM","audio.INTERNAL_PDM"]]), "mode");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['audio_setpinout'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Audio.setPinout");
    this.appendValueInput("bclk")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("bclk");
    this.appendValueInput("lrck")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("lrck");
    this.appendValueInput("dout")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("dout");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['audio_update'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Audio.update");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['audio_playsd'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Audio.playSd");
    this.appendValueInput("path")
        .setCheck("String")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("path");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['audio_setgain'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Audio.setGain");
    this.appendValueInput("gain")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("gain");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['audio_getgain'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Audio.getGain");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['audio_isrunning'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Audio.isRunning");
    this.setInputsInline(true);
    this.setOutput(true, "Boolean");
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['audio_stop'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Audio.stop");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['setloop'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("setLoop");
    this.appendStatementInput("func")
        .setCheck(null)
        .setAlign(Blockly.ALIGN_RIGHT);
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['esp32_reboot'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Esp32.reboot");
    this.setPreviousStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['esp32_restart'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Esp32.restart");
    this.setPreviousStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['update'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("update");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};

Blockly.Blocks['setloop'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("setLoop");
    this.appendStatementInput("func")
        .setCheck(null)
        .setAlign(Blockly.ALIGN_RIGHT);
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
 this.setTooltip("");
 this.setHelpUrl("");
  }
};