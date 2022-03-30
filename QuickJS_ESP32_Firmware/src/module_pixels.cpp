#include <Arduino.h>
#include "quickjs.h"
#include "module_pixels.h"
#include "module_type.h"
#include <Adafruit_NeoPixel.h>

#define DEFAULT_NUMPIXELS  25

static Adafruit_NeoPixel *pixels = NULL;
static uint16_t num_colors = 0;
static uint32_t *last_colors = NULL;
static bool onoff = true;

static JSValue esp32_pixels_begin(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  uint32_t pin;
  JS_ToUint32(ctx, &pin, argv[0]);
  uint32_t num = DEFAULT_NUMPIXELS;
  if( argc >= 2 )
    JS_ToUint32(ctx, &num, argv[1]);

  if( pixels != NULL ){
    delete pixels;
    pixels = NULL;
  }
  if( last_colors != NULL ){
    free(last_colors);
    last_colors = NULL;
  }

  last_colors = (uint32_t*)calloc(num, sizeof(uint32_t));
  if( last_colors == NULL )
    return JS_EXCEPTION;
  num_colors = num;
  pixels = new Adafruit_NeoPixel(num_colors, pin, NEO_GRB + NEO_KHZ800);
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
  if( pixels == NULL ){
    return JS_EXCEPTION;
  }

  onoff = JS_ToBool(ctx, argv[0]);

  if( onoff ){
    for( int i = 0 ; i < num_colors ; i++ )
      pixels->setPixelColor(i, last_colors[i]);
  }else{
    for( int i = 0 ; i < num_colors ; i++ )
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
  if( pixels == NULL ){
    return JS_EXCEPTION;
  }

  uint32_t index, color;
  JS_ToUint32(ctx, &index, argv[0]);
  JS_ToUint32(ctx, &color, argv[1]);

  onoff = true;
  pixels->setPixelColor(index, color);
  pixels->show();

  return JS_UNDEFINED;
}

static JSValue esp32_pixels_getPixelColor(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  if( pixels == NULL ){
    return JS_EXCEPTION;
  }

  uint32_t index;
  JS_ToUint32(ctx, &index, argv[0]);

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

void endModule_pixels(void)
{
  if( pixels != NULL ){
    delete pixels;
    pixels = NULL;
  }
  if( last_colors != NULL ){
    free(last_colors);
    last_colors = NULL;
  }
}

JsModuleEntry pixels_module = {
  NULL,
  addModule_pixels,
  NULL,
  endModule_pixels
};