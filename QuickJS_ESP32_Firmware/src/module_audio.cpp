#include <Arduino.h>
#include "quickjs.h"
#include "main_config.h"
#include "module_type.h"
#include "module_utils.h"
#include "module_audio.h"

#include <driver/i2s.h>
#include "AudioFileSourceHTTPStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourceSD.h"

#define DEFAULT_AUDIO_GAIN  40.0

static AudioOutputI2S *out = NULL;
static AudioGeneratorMP3 *mp3 = NULL;
static AudioFileSourceSD *file_sd = NULL;
static AudioFileSourceHTTPStream *file_http = NULL;
static AudioFileSourceBuffer *buff = NULL;
static float audio_gain = DEFAULT_AUDIO_GAIN;
static bool audio_paused = false;

#define AUDIO_BUFFER_SIZE 1024

static void audio_source_dispose(void){
  if( mp3 != NULL ){
    mp3->stop();
  }

  if( buff != NULL ){
    buff->close();
  }
  if( file_sd != NULL ){
    file_sd->close();
  }
  if( file_http != NULL ){
    file_http->close();
  }

  if( mp3 != NULL ){
    delete mp3;
    mp3 = NULL;
  }

  if( buff != NULL ){
    delete buff;
    buff = NULL;
  }
  if( file_sd != NULL ){
    delete file_sd;
    file_sd = NULL;
  }
  if( file_http != NULL ){
    delete file_http;
    file_http = NULL;
  }

  audio_paused = false;
}

static JSValue audio_begin(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  audio_source_dispose();
  if( out != NULL ){
    out->stop();
    delete out;
    out = NULL;
  }

  int32_t output_mode = AudioOutputI2S::EXTERNAL_I2S;
  if( argc >= 1 )
    JS_ToInt32(ctx, &output_mode, argv[0]);

  out = new AudioOutputI2S(I2S_NUM_0, output_mode);
  out->SetOutputModeMono(true);
  out->SetGain(audio_gain / 100.0);
  
  return JS_UNDEFINED;
}

static JSValue audio_setPinout(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  if( out == NULL )
    return JS_EXCEPTION;

  uint32_t bclk, lrck, dout;
  JS_ToUint32(ctx, &bclk, argv[0]);
  JS_ToUint32(ctx, &lrck, argv[1]);
  JS_ToUint32(ctx, &dout, argv[2]);

  bool ret = out->SetPinout(bclk, lrck, dout);

  return JS_NewBool(ctx, ret);
}

static JSValue audio_update(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  if( mp3 != NULL ){
    if (mp3->isRunning()) {
      if( !audio_paused ){
      if (!mp3->loop())
        mp3->stop();
    }
  }
  }

  return JS_UNDEFINED;
}

static JSValue audio_pause(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv, int magic)
{
  if( mp3 != NULL ){
    if (mp3->isRunning()) {
      if( magic == 0 ){
        out->SetGain(audio_gain/100.0);
        audio_paused = false;
      }else{
        out->SetGain(0/100.0);
        out->flush();
        audio_paused = true;
      }
    }
  }

  return JS_UNDEFINED;
}

static JSValue audio_playUrl(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  if( out == NULL )
    return JS_EXCEPTION;

  const char *url = JS_ToCString(ctx, argv[0]);
  if( url == NULL )
    return JS_EXCEPTION;

  uint32_t bufsize = AUDIO_BUFFER_SIZE;
  if( argc >= 2 )
    JS_ToUint32(ctx, &bufsize, argv[1]);

  audio_source_dispose();

  file_http = new AudioFileSourceHTTPStream(url);
  buff = new AudioFileSourceBuffer(file_http, bufsize);
  mp3 = new AudioGeneratorMP3();
  bool ret = mp3->begin(buff, out);

  return JS_NewBool(ctx, ret);
}

static JSValue audio_playSd(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  if( out == NULL )
    return JS_EXCEPTION;
    
  const char *path = JS_ToCString(ctx, argv[0]);
  if( path == NULL )
    return JS_EXCEPTION;

  audio_source_dispose();

  file_sd = new AudioFileSourceSD(path);
  if( !file_sd->isOpen() ){
    delete file_sd;
    file_sd = NULL;
    return JS_EXCEPTION;
  }
  mp3 = new AudioGeneratorMP3();
  bool ret = mp3->begin(file_sd, out);

  return JS_NewBool(ctx, ret);
}

static JSValue audio_setGain(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  if( out == NULL )
    return JS_EXCEPTION;
    
  double gain;
  JS_ToFloat64(ctx, &gain, argv[0]);

  audio_gain = gain;
  bool ret = out->SetGain(audio_gain/100.0);

  return JS_NewBool(ctx, ret);
}

static JSValue audio_getGain(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  return JS_NewFloat64(ctx, audio_gain);
}

static JSValue audio_isRunning(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  if( out == NULL || mp3 == NULL )
    return JS_NewBool(ctx, false);

  bool is_running = mp3->isRunning();

  return JS_NewBool(ctx, is_running);
}

static JSValue audio_stop(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  audio_source_dispose();

  return JS_UNDEFINED;
}

static const JSCFunctionListEntry audio_funcs[] = {
    JSCFunctionListEntry{"begin", 0, JS_DEF_CFUNC, 0, {
                           func : {1, JS_CFUNC_generic, audio_begin}
                         }},
    JSCFunctionListEntry{"setPinout", 0, JS_DEF_CFUNC, 0, {
                           func : {3, JS_CFUNC_generic, audio_setPinout}
                         }},
    JSCFunctionListEntry{"update", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, audio_update}
                         }},
    JSCFunctionListEntry{
        "pause", 0, JS_DEF_CFUNC, 1, {
          func : {0, JS_CFUNC_generic_magic, {generic_magic : audio_pause}}
        }},
    JSCFunctionListEntry{
        "resume", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic_magic, {generic_magic : audio_pause}}
        }},
    JSCFunctionListEntry{"playUrl", 0, JS_DEF_CFUNC, 0, {
                           func : {2, JS_CFUNC_generic, audio_playUrl}
                         }},
    JSCFunctionListEntry{"playSd", 0, JS_DEF_CFUNC, 0, {
                           func : {1, JS_CFUNC_generic, audio_playSd}
                         }},
    JSCFunctionListEntry{"setGain", 0, JS_DEF_CFUNC, 0, {
                           func : {1, JS_CFUNC_generic, audio_setGain}
                         }},
    JSCFunctionListEntry{"getGain", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, audio_getGain}
                         }},
    JSCFunctionListEntry{"isRunning", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, audio_isRunning}
                         }},
    JSCFunctionListEntry{"stop", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, audio_stop}
                         }},
    JSCFunctionListEntry{
        "EXTERNAL_I2S", 0, JS_DEF_PROP_INT32, 0, {
          i32 : AudioOutputI2S::EXTERNAL_I2S
        }},
    JSCFunctionListEntry{
        "INTERNAL_DAC", 0, JS_DEF_PROP_INT32, 0, {
          i32 : AudioOutputI2S::INTERNAL_DAC
        }},
    JSCFunctionListEntry{
        "INTERNAL_PDM", 0, JS_DEF_PROP_INT32, 0, {
          i32 : AudioOutputI2S::INTERNAL_PDM
        }},
};

JSModuleDef *addModule_audio(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;

  mod = JS_NewCModule(ctx, "Audio", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, audio_funcs,
                            sizeof(audio_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, audio_funcs,
        sizeof(audio_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

void endModule_audio(void)
{
  audio_source_dispose();
  if( out != NULL ){
    out->stop();
    delete out;
    out = NULL;
}
  audio_gain = DEFAULT_AUDIO_GAIN;
}

JsModuleEntry audio_module = {
  NULL,
  addModule_audio,
  NULL,
  endModule_audio
};
