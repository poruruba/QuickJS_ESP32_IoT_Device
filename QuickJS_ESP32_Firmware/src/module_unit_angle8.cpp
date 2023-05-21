#include <Arduino.h>
#include "main_config.h"

#ifdef _UNIT_ANGLE8_ENABLE_

#include "quickjs.h"
#include "module_unit_angle8.h"
#include "M5_ANGLE8.h"

static M5_ANGLE8 angle8;

static JSValue unit_angle8_begin(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  if( !angle8.begin(ANGLE8_I2C_ADDR) )
    return JS_EXCEPTION;

  return JS_UNDEFINED;
}

static JSValue unit_angle8_getDigitalInput(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  bool value = angle8.getDigitalInput();

  return JS_NewBool(ctx, value);
}

static JSValue unit_angle8_getAnalogInput(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t ch;
  JS_ToUint32(ctx, &ch, argv[0]);

  uint16_t value = angle8.getAnalogInput((uint8_t)ch, _12bit);

  return JS_NewUint32(ctx, value);
}

static JSValue unit_angle8_setLEDColor(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t ch, color, bright;
  JS_ToUint32(ctx, &ch, argv[0]);
  JS_ToUint32(ctx, &color, argv[1]);
  JS_ToUint32(ctx, &bright, argv[2]);

  bool result = angle8.setLEDColor((uint8_t)ch, color, (uint8_t)bright);

  return JS_NewBool(ctx, result);
}

static const JSCFunctionListEntry unit_angle8_funcs[] = {
    JSCFunctionListEntry{
        "begin", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, unit_angle8_begin}
        }},
    JSCFunctionListEntry{
        "getDigitalInput", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, unit_angle8_getDigitalInput}
        }},
    JSCFunctionListEntry{
        "getAnalogInput", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, unit_angle8_getAnalogInput}
        }},
    JSCFunctionListEntry{
        "setLEDColor", 0, JS_DEF_CFUNC, 0, {
          func : {3, JS_CFUNC_generic, unit_angle8_setLEDColor}
        }},
};

JSModuleDef *addModule_unit_angle8(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "UnitAngle8", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, unit_angle8_funcs,
                            sizeof(unit_angle8_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, unit_angle8_funcs,
        sizeof(unit_angle8_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

JsModuleEntry unit_angle8_module = {
  NULL,
  addModule_unit_angle8,
  NULL,
  NULL
};

#endif