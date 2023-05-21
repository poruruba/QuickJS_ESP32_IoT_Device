#include <Arduino.h>
#include "main_config.h"

#ifdef _UNIT_COLOR_ENABLE_

#include "quickjs.h"
#include "module_unit_color.h"
#include "Adafruit_TCS34725.h"

static Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

static JSValue unit_color_begin(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  if( !tcs.begin() )
    return JS_EXCEPTION;

  tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);
  tcs.setGain(TCS34725_GAIN_4X);

  return JS_UNDEFINED;
}

static JSValue unit_color_getRawData(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint16_t clear, red, green, blue;
  tcs.getRawData(&red, &green, &blue, &clear); 

  JSValue obj = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, obj, "red", JS_NewUint32(ctx, red));
  JS_SetPropertyStr(ctx, obj, "green", JS_NewUint32(ctx, green));
  JS_SetPropertyStr(ctx, obj, "blue", JS_NewUint32(ctx, blue));
  JS_SetPropertyStr(ctx, obj, "clear", JS_NewUint32(ctx, clear));
  return obj;
}
static JSValue unit_color_getRgb(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint16_t clear, red, green, blue;
  tcs.getRawData(&red, &green, &blue, &clear); 

  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;

  JSValue obj = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, obj, "red", JS_NewUint32(ctx, (uint8_t)r));
  JS_SetPropertyStr(ctx, obj, "green", JS_NewUint32(ctx, (uint8_t)g));
  JS_SetPropertyStr(ctx, obj, "blue", JS_NewUint32(ctx, (uint8_t)b));
  return obj;
}

static const JSCFunctionListEntry unit_color_funcs[] = {
    JSCFunctionListEntry{
        "begin", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, unit_color_begin}
        }},
    JSCFunctionListEntry{
        "getRawData", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, unit_color_getRawData}
        }},
    JSCFunctionListEntry{
        "getRgb", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, unit_color_getRgb}
        }},
};

JSModuleDef *addModule_unit_color(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "UnitColor", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, unit_color_funcs,
                            sizeof(unit_color_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, unit_color_funcs,
        sizeof(unit_color_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

JsModuleEntry unit_color_module = {
  NULL,
  addModule_unit_color,
  NULL,
  NULL
};

#endif
