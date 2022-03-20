#include <Arduino.h>
#include "quickjs.h"
#include "module_pixels.h"
#include "module_type.h"
#include <Adafruit_NeoPixel.h>

#if defined(ARDUINO_M5Stack_ATOM)
#include <M5Atom.h>
#define NUMPIXELS  25
#elif defined(ARDUINO_M5Stick_C)
#define NUMPIXELS  4
#else
#define NUMPIXELS  1
#endif

static Adafruit_NeoPixel *pixels = NULL;
static uint32_t last_colors[NUMPIXELS] = { 0 };
static bool onoff = true;

static JSValue esp32_pixels_begin(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  uint32_t pin;
  JS_ToUint32(ctx, &pin, argv[0]);

  if( pixels == NULL )
    pixels = new Adafruit_NeoPixel(NUMPIXELS, pin, NEO_GRB + NEO_KHZ800);
  else
    pixels->setPin(pin);

  pixels->begin();

  return JS_UNDEFINED;
}

static JSValue esp32_pixels_clear(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  if( pixels == NULL ){
    return JS_EXCEPTION;
  }
  
  pixels->clear();
  pixels->show();

  return JS_UNDEFINED;
}

static JSValue esp32_pixels_setOnoff(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  onoff = JS_ToBool(ctx, argv[0]);

  if( pixels == NULL ){
    return JS_EXCEPTION;
  }

  if( onoff ){
    for( int i = 0 ; i < NUMPIXELS ; i++ )
      pixels->setPixelColor(i, last_colors[i]);
  }else{
    for( int i = 0 ; i < NUMPIXELS ; i++ )
      last_colors[i] = pixels->getPixelColor(i);
    pixels->clear();
  }
  pixels->show();

  return JS_UNDEFINED;
}

static JSValue esp32_pixels_getOnoff(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  return JS_NewBool(ctx, onoff);
}

static JSValue esp32_pixels_setPixelColor(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t index, color;
  JS_ToUint32(ctx, &index, argv[0]);
  JS_ToUint32(ctx, &color, argv[1]);

  if( pixels == NULL ){
    return JS_EXCEPTION;
  }

  onoff = true;
  pixels->setPixelColor(index, color);
  pixels->show();

  return JS_UNDEFINED;
}

static JSValue esp32_pixels_getPixelColor(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t index;
  JS_ToUint32(ctx, &index, argv[0]);

  if( pixels == NULL ){
    return JS_EXCEPTION;
  }

  uint32_t ret = pixels->getPixelColor(index);

  return JS_NewUint32(ctx, ret);
}

static const JSCFunctionListEntry pixels_funcs[] = {
    JSCFunctionListEntry{
        "begin", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, esp32_pixels_begin}
        }},
    JSCFunctionListEntry{
        "clear", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, esp32_pixels_clear}
        }},
    JSCFunctionListEntry{
        "setOnoff", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, esp32_pixels_setOnoff}
        }},
    JSCFunctionListEntry{
        "getOnoff", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, esp32_pixels_getOnoff}
        }},
    JSCFunctionListEntry{
        "setPixelColor", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, esp32_pixels_setPixelColor}
        }},
    JSCFunctionListEntry{
        "getPixelColor", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, esp32_pixels_getPixelColor}
        }},
};

JSModuleDef *addModule_pixels(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "Pixels", [](JSContext *ctx, JSModuleDef *m){
          return JS_SetModuleExportList(
                      ctx, m, pixels_funcs,
                      sizeof(pixels_funcs) / sizeof(JSCFunctionListEntry));
          });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, pixels_funcs,
        sizeof(pixels_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

JsModuleEntry pixels_module = {
  NULL,
  addModule_pixels,
  NULL,
  NULL
};