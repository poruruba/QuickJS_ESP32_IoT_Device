#include <Arduino.h>
#include "quickjs.h"
#include "module_type.h"
#include "module_utils.h"
#include "module_audio.h"

#include <driver/i2s.h>
#include "AudioFileSourceHTTPStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourceSD.h"

static AudioOutputI2S *out;

static AudioGeneratorMP3 *mp3 = NULL;
static AudioFileSourceSD *file_sd = NULL;
static AudioFileSourceHTTPStream *file_http = NULL;
static AudioFileSourceBuffer *buff = NULL;
static float audio_gain = 40.0;

#define AUDIO_BUFFER_SIZE 1024

static void audio_dispose(void){
  if( mp3 != NULL ){
    mp3->stop();
    delete mp3;
    mp3 = NULL;
  }

  if( buff != NULL ){
    buff->close();
    delete buff;
    buff = NULL;
  }
  if( file_sd != NULL ){
    file_sd->close();
    delete file_sd;
    file_sd = NULL;
  }
  if( file_http != NULL ){
    file_http->close();
    delete file_http;
    file_http = NULL;
  }
}

static JSValue audio_begin(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
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
      if (!mp3->loop())
        mp3->stop();
    }
  }

  return 0;
}

static JSValue audio_playUrl(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  const char *url = JS_ToCString(ctx, argv[0]);
  if( url == NULL )
    return JS_EXCEPTION;

  uint32_t bufsize = AUDIO_BUFFER_SIZE;
  if( argc >= 2 )
    JS_ToUint32(ctx, &bufsize, argv[1]);

  audio_dispose();

  file_http = new AudioFileSourceHTTPStream(url);
  buff = new AudioFileSourceBuffer(file_http, bufsize);
  mp3 = new AudioGeneratorMP3();
  bool ret = mp3->begin(buff, out);

  return JS_NewBool(ctx, ret);
}

static JSValue audio_playSd(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  const char *path = JS_ToCString(ctx, argv[0]);
  if( path == NULL )
    return JS_EXCEPTION;

  audio_dispose();

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
  if( mp3 == NULL )
    return JS_NewBool(ctx, false);

  bool is_running = mp3->isRunning();

  return JS_NewBool(ctx, is_running);
}

static JSValue audio_stop(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  audio_dispose();

  return JS_UNDEFINED;
}

static const JSCFunctionListEntry audio_funcs[] = {
    JSCFunctionListEntry{"begin", 0, JS_DEF_CFUNC, 0, {
                           func : {3, JS_CFUNC_generic, audio_begin}
                         }},
    JSCFunctionListEntry{"update", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, audio_update}
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
  audio_dispose();
}

long initializeModule_audio(void)
{
  out = new AudioOutputI2S(I2S_NUM_0);
  out->SetOutputModeMono(true);
  out->SetGain(audio_gain / 100.0);

  return 0;
}

JsModuleEntry audio_module = {
  initializeModule_audio,
  addModule_audio,
  NULL,
  endModule_audio
};
