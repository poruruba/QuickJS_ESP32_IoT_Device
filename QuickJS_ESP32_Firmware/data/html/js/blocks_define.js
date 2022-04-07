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
        .appendField("btn")
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
        .appendField("onoff");
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
        .appendField(new Blockly.FieldDropdown([["Audio","Audio"], ["Gpio","Gpio"], ["Imu","Imu"], ["Utils","Utils"]]), "module");
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