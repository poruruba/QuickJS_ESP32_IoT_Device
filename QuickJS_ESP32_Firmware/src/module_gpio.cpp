#include <Arduino.h>
#include "quickjs.h"
#include "module_type.h"

static JSValue esp32_gpio_mode(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  uint32_t pin, mode;
  JS_ToUint32(ctx, &pin, argv[0]);
  JS_ToUint32(ctx, &mode, argv[1]);
  pinMode(pin, mode);
  return JS_UNDEFINED;
}

static JSValue esp32_gpio_digital_read(JSContext *ctx, JSValueConst jsThis,
                                       int argc, JSValueConst *argv)
{
  uint32_t pin;
  JS_ToUint32(ctx, &pin, argv[0]);
  return JS_NewInt32(ctx, digitalRead(pin));
}

static JSValue esp32_gpio_analog_read(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  uint32_t pin;
  JS_ToUint32(ctx, &pin, argv[0]);
  return JS_NewUint32(ctx, analogRead(pin));
}

static JSValue esp32_gpio_digital_write(JSContext *ctx, JSValueConst jsThis,
                                        int argc, JSValueConst *argv)
{
  uint32_t pin, value;
  JS_ToUint32(ctx, &pin, argv[0]);
  JS_ToUint32(ctx, &value, argv[1]);
  digitalWrite(pin, value);
  return JS_UNDEFINED;
}

static const JSCFunctionListEntry gpio_funcs[] = {
    JSCFunctionListEntry{"pinMode", 0, JS_DEF_CFUNC, 0, {
                           func : {2, JS_CFUNC_generic, esp32_gpio_mode}
                         }},
    JSCFunctionListEntry{
        "analogRead", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, esp32_gpio_analog_read}
        }},
    JSCFunctionListEntry{
        "digitalRead", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, esp32_gpio_digital_read}
        }},
    JSCFunctionListEntry{
        "digitalWrite", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, esp32_gpio_digital_write}
        }},
    JSCFunctionListEntry{
        "LOW", 0, JS_DEF_PROP_INT32, 0, {
          i32 : LOW
        }},
    JSCFunctionListEntry{
        "HIGH", 0, JS_DEF_PROP_INT32, 0, {
          i32 : HIGH
        }},
    JSCFunctionListEntry{
        "INPUT", 0, JS_DEF_PROP_INT32, 0, {
          i32 : INPUT
        }},
    JSCFunctionListEntry{
        "OUTPUT", 0, JS_DEF_PROP_INT32, 0, {
          i32 : OUTPUT
        }},
    JSCFunctionListEntry{
        "PULLUP", 0, JS_DEF_PROP_INT32, 0, {
          i32 : PULLUP
        }},
    JSCFunctionListEntry{
        "INPUT_PULLUP", 0, JS_DEF_PROP_INT32, 0, {
          i32 : INPUT_PULLUP
        }},
    JSCFunctionListEntry{
        "PULLDOWN", 0, JS_DEF_PROP_INT32, 0, {
          i32 : PULLDOWN
        }},
    JSCFunctionListEntry{
        "INPUT_PULLDOWN", 0, JS_DEF_PROP_INT32, 0, {
          i32 : INPUT_PULLDOWN
        }},
    JSCFunctionListEntry{
        "OPEN_DRAIN", 0, JS_DEF_PROP_INT32, 0, {
          i32 : OPEN_DRAIN
        }},
    JSCFunctionListEntry{
        "OUTPUT_OPEN_DRAIN", 0, JS_DEF_PROP_INT32, 0, {
          i32 : OUTPUT_OPEN_DRAIN
        }},
};

JSModuleDef *addModule_gpio(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;

  mod = JS_NewCModule(ctx, "Gpio", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, gpio_funcs,
                            sizeof(gpio_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, gpio_funcs,
        sizeof(gpio_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

JsModuleEntry gpio_module = {
  NULL,
  addModule_gpio,
  NULL,
  NULL
};
