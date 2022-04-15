#include <Arduino.h>
#include "quickjs.h"
#include "module_ledc.h"
#include "module_type.h"
#include "esp32-hal-ledc.h"

#define NUM_LEDC_CHANNEL  3
static bool is_beeping[NUM_LEDC_CHANNEL] = { false, false, false };
static uint32_t beep_end[NUM_LEDC_CHANNEL];

static JSValue esp32_ledc_setup(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  uint32_t channel, resolution;
  double freq;
  JS_ToUint32(ctx, &channel, argv[0]);
  JS_ToFloat64(ctx, &freq, argv[1]);
  JS_ToUint32(ctx, &resolution, argv[2]);

  double ret = ledcSetup(channel, freq, resolution);

  return JS_NewFloat64(ctx, ret);
}

static JSValue esp32_ledc_attachPin(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t channel, pin;
  JS_ToUint32(ctx, &pin, argv[0]);
  JS_ToUint32(ctx, &channel, argv[1]);

  ledcAttachPin(pin, channel);

  return JS_UNDEFINED;
}

static JSValue esp32_ledc_detachPin(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t pin;
  JS_ToUint32(ctx, &pin, argv[0]);

  ledcDetachPin(pin);

  return JS_UNDEFINED;
}

static JSValue esp32_ledc_write(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t channel, duty;
  JS_ToUint32(ctx, &channel, argv[0]);
  JS_ToUint32(ctx, &duty, argv[1]);

  ledcWrite(channel, duty);

  return JS_UNDEFINED;
}

static JSValue esp32_ledc_writeTone(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t channel;
  double freq;
  JS_ToUint32(ctx, &channel, argv[0]);
  JS_ToFloat64(ctx, &freq, argv[1]);

  double ret = ledcWriteTone(channel, freq);

  return JS_NewFloat64(ctx, ret);
}

static JSValue esp32_ledc_writeNote(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t channel, note, octave;
  JS_ToUint32(ctx, &channel, argv[0]);
  JS_ToUint32(ctx, &note, argv[1]);
  JS_ToUint32(ctx, &octave, argv[2]);

  double ret = ledcWriteNote(channel, (note_t)note, octave);

  return JS_NewFloat64(ctx, ret);
}

static JSValue esp32_ledc_read(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t channel;
  JS_ToUint32(ctx, &channel, argv[0]);

  uint32_t ret = ledcRead(channel);

  return JS_NewUint32(ctx, ret);
}

static JSValue esp32_ledc_readFreq(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t channel;
  JS_ToUint32(ctx, &channel, argv[0]);

  double ret = ledcReadFreq(channel);

  return JS_NewFloat64(ctx, ret);
}

static JSValue esp32_ledc_beepNote(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t channel, note, octave;
  JS_ToUint32(ctx, &channel, argv[0]);
  JS_ToUint32(ctx, &note, argv[1]);
  JS_ToUint32(ctx, &octave, argv[2]);
  uint32_t duration;
  JS_ToUint32(ctx, &duration, argv[3]);

  if( channel >= NUM_LEDC_CHANNEL )
    return JS_EXCEPTION;

  is_beeping[channel] = true;
  beep_end[channel] = millis() + duration;

  double ret = ledcWriteNote(channel, (note_t)note, octave);

  return JS_NewFloat64(ctx, ret);
}

static const JSCFunctionListEntry ledc_funcs[] = {
    JSCFunctionListEntry{
        "setup", 0, JS_DEF_CFUNC, 0, {
          func : {3, JS_CFUNC_generic, esp32_ledc_setup}
        }},
    JSCFunctionListEntry{
        "attachPin", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, esp32_ledc_attachPin}
        }},
    JSCFunctionListEntry{
        "detachPin", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, esp32_ledc_detachPin}
        }},
    JSCFunctionListEntry{
        "write", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, esp32_ledc_write}
        }},
    JSCFunctionListEntry{
        "writeTone", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, esp32_ledc_writeTone}
        }},
    JSCFunctionListEntry{
        "writeNote", 0, JS_DEF_CFUNC, 0, {
          func : {3, JS_CFUNC_generic, esp32_ledc_writeNote}
        }},
    JSCFunctionListEntry{
        "read", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, esp32_ledc_read}
        }},
    JSCFunctionListEntry{
        "readFreq", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, esp32_ledc_readFreq}
        }},
    JSCFunctionListEntry{
        "beepNote", 0, JS_DEF_CFUNC, 0, {
          func : {4, JS_CFUNC_generic, esp32_ledc_beepNote}
        }},
        
    JSCFunctionListEntry{
        "NOTE_C", 0, JS_DEF_PROP_INT32, 0, {
          i32 : NOTE_C
        }},
    JSCFunctionListEntry{
        "NOTE_Cs", 0, JS_DEF_PROP_INT32, 0, {
          i32 : NOTE_Cs
        }},
    JSCFunctionListEntry{
        "NOTE_D", 0, JS_DEF_PROP_INT32, 0, {
          i32 : NOTE_D
        }},
    JSCFunctionListEntry{
        "NOTE_Eb", 0, JS_DEF_PROP_INT32, 0, {
          i32 : NOTE_Eb
        }},
    JSCFunctionListEntry{
        "NOTE_E", 0, JS_DEF_PROP_INT32, 0, {
          i32 : NOTE_E
        }},
    JSCFunctionListEntry{
        "NOTE_F", 0, JS_DEF_PROP_INT32, 0, {
          i32 : NOTE_F
        }},
    JSCFunctionListEntry{
        "NOTE_Fs", 0, JS_DEF_PROP_INT32, 0, {
          i32 : NOTE_Fs
        }},
    JSCFunctionListEntry{
        "NOTE_G", 0, JS_DEF_PROP_INT32, 0, {
          i32 : NOTE_G
        }},
    JSCFunctionListEntry{
        "NOTE_Gs", 0, JS_DEF_PROP_INT32, 0, {
          i32 : NOTE_Gs
        }},
    JSCFunctionListEntry{
        "NOTE_A", 0, JS_DEF_PROP_INT32, 0, {
          i32 : NOTE_A
        }},
    JSCFunctionListEntry{
        "NOTE_Bb", 0, JS_DEF_PROP_INT32, 0, {
          i32 : NOTE_Bb
        }},
    JSCFunctionListEntry{
        "NOTE_B", 0, JS_DEF_PROP_INT32, 0, {
          i32 : NOTE_B
        }},
    JSCFunctionListEntry{
        "NOTE_MAX", 0, JS_DEF_PROP_INT32, 0, {
          i32 : NOTE_MAX
        }},
};

JSModuleDef *addModule_ledc(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "Ledc", [](JSContext *ctx, JSModuleDef *m){
          return JS_SetModuleExportList(
                      ctx, m, ledc_funcs,
                      sizeof(ledc_funcs) / sizeof(JSCFunctionListEntry));
          });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, ledc_funcs,
        sizeof(ledc_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

void loopModule_ledc(void){
  for( int i = 0 ; i < NUM_LEDC_CHANNEL ; i++ ){
    if( is_beeping[i] ){
      if( beep_end[i] <= millis() ){
        is_beeping[i] = false;
        ledcWrite(i, 0);
      }
    }
  }
}

void endModule_ledc(void){
  for( int i = 0 ; i < NUM_LEDC_CHANNEL ; i++ ){
    if( is_beeping[i] ){
      is_beeping[i] = false;
      ledcWrite(i, 0);
    }
  }
}

JsModuleEntry ledc_module = {
  NULL,
  addModule_ledc,
  loopModule_ledc,
  endModule_ledc
};