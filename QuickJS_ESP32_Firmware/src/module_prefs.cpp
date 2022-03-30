#include <Arduino.h>
#include <Preferences.h>
#include "main_config.h"
#include "quickjs.h"
#include "module_utils.h"
#include "module_prefs.h"

Preferences2 preferences;

static JSValue esp32_prefs_begin(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  const char *name = JS_ToCString(ctx, argv[0]);
  if( name == NULL )
    return JS_EXCEPTION;

  bool readonly = false;
  if( argc >= 2 )
    JS_ToBool(ctx, argv[1]);

  bool ret = preferences.begin(name, readonly);

  JS_FreeCString(ctx, name);

  return JS_NewBool(ctx, ret);
}

static JSValue esp32_prefs_end(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  preferences.end();

  return JS_UNDEFINED;
}

static JSValue esp32_prefs_clear(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  bool ret = preferences.clear();

  return JS_NewBool(ctx, ret);
}

static JSValue esp32_prefs_remove(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  const char *key = JS_ToCString(ctx, argv[0]);
  if( key == NULL )
    return JS_EXCEPTION;

  bool ret = preferences.remove(key);

  JS_FreeCString(ctx, key);

  return JS_NewBool(ctx, ret);
}

static JSValue esp32_prefs_isKey(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  const char *key = JS_ToCString(ctx, argv[0]);
  if( key == NULL )
    return JS_EXCEPTION;

  bool ret = preferences.isKey(key);

  JS_FreeCString(ctx, key);

  return JS_NewBool(ctx, ret);
}

static JSValue esp32_prefs_getType(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  const char *key = JS_ToCString(ctx, argv[0]);
  if( key == NULL )
    return JS_EXCEPTION;

  uint32_t ret = preferences.getType(key);

  JS_FreeCString(ctx, key);

  return JS_NewUint32(ctx, ret);
}

static JSValue esp32_prefs_putNumber(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  const char *key = JS_ToCString(ctx, argv[0]);
  if( key == NULL )
    return JS_EXCEPTION;

  int32_t value;
  JS_ToInt32(ctx, &value, argv[1]);

  size_t ret = preferences.putLong(key, value);

  JS_FreeCString(ctx, key);

  return JS_NewUint32(ctx, ret);
}

static JSValue esp32_prefs_getNumber(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  const char *key = JS_ToCString(ctx, argv[0]);
  if( key == NULL )
    return JS_EXCEPTION;

  int32_t def = 0;
  if( argc >= 2 )
    JS_ToInt32(ctx, &def, argv[1]);

  int32_t ret = preferences.getLong(key, def);

  JS_FreeCString(ctx, key);

  return JS_NewInt32(ctx, ret);
}

static JSValue esp32_prefs_putDouble(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  const char *key = JS_ToCString(ctx, argv[0]);
  if( key == NULL )
    return JS_EXCEPTION;

  double value;
  JS_ToFloat64(ctx, &value, argv[1]);

  size_t ret = preferences.putDouble(key, value);

  JS_FreeCString(ctx, key);

  return JS_NewUint32(ctx, ret);
}

static JSValue esp32_prefs_getDouble(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  const char *key = JS_ToCString(ctx, argv[0]);
  if( key == NULL )
    return JS_EXCEPTION;

  double def = 0.0;
  if( argc >= 2 )
    JS_ToFloat64(ctx, &def, argv[1]);

  double ret = preferences.getDouble(key, def);

  JS_FreeCString(ctx, key);

  return JS_NewFloat64(ctx, ret);
}

static JSValue esp32_prefs_putBool(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  const char *key = JS_ToCString(ctx, argv[0]);
  if( key == NULL )
    return JS_EXCEPTION;

  bool value;
  value = JS_ToBool(ctx, argv[1]);

  size_t ret = preferences.putBool(key, value);

  JS_FreeCString(ctx, key);

  return JS_NewUint32(ctx, ret);
}

static JSValue esp32_prefs_getBool(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  const char *key = JS_ToCString(ctx, argv[0]);
  if( key == NULL )
    return JS_EXCEPTION;

  bool def = false;
  if( argc >= 2 )
    def = JS_ToBool(ctx, argv[1]);

  bool ret = preferences.getBool(key, def);

  JS_FreeCString(ctx, key);

  return JS_NewBool(ctx, ret);
}

static JSValue esp32_prefs_putString(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  const char *key = JS_ToCString(ctx, argv[0]);
  if( key == NULL )
    return JS_EXCEPTION;

  const char *value = JS_ToCString(ctx, argv[1]);
  if( value == NULL ){
    JS_FreeCString(ctx, key);
    return JS_EXCEPTION;
  }

  size_t ret = preferences.putString(key, value);

  JS_FreeCString(ctx, key);
  JS_FreeCString(ctx, value);

  return JS_NewUint32(ctx, ret);
}

static JSValue esp32_prefs_getString(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  const char *key = JS_ToCString(ctx, argv[0]);
  if( key == NULL )
    return JS_EXCEPTION;

  String ret = preferences.getString(key);

  JS_FreeCString(ctx, key);

  return JS_NewString(ctx, ret.c_str());
}

static JSValue esp32_prefs_putBytes(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  const char *key = JS_ToCString(ctx, argv[0]);
  if( key == NULL )
    return JS_EXCEPTION;

  uint8_t *p_buffer;
  uint8_t unit_size;
  uint32_t bsize;
  JSValue vbuffer = getArrayBuffer(ctx, argv[1], (void**)&p_buffer, &unit_size, &bsize);
  if( JS_IsNull(vbuffer) ){
    JS_FreeCString(ctx, key);
    return JS_EXCEPTION;
  }
  if( unit_size != 1 ){
    JS_FreeCString(ctx, key);
    JS_FreeValue(ctx, vbuffer);
    return JS_EXCEPTION;
  }
  if( bsize > PREF_MAX_BYTES_LEN ){
    JS_FreeCString(ctx, key);
    JS_FreeValue(ctx, vbuffer);
    return JS_EXCEPTION;
  }

  size_t ret = preferences.putBytes(key, p_buffer, bsize);

  JS_FreeCString(ctx, key);
  JS_FreeValue(ctx, vbuffer);

  return JS_NewUint32(ctx, ret);
}

static JSValue esp32_prefs_getBytes(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  const char *key = JS_ToCString(ctx, argv[0]);
  if( key == NULL )
    return JS_EXCEPTION;

  uint32_t def = PREF_MAX_BYTES_LEN;
  if( argc >= 2 )
    JS_ToUint32(ctx, &def, argv[1]);

  uint8_t *p_buffer = (uint8_t*)malloc(def);
  if( p_buffer == NULL ){
    JS_FreeCString(ctx, key);
    return JS_EXCEPTION;
  }

  int32_t ret = preferences.getBytes(key, p_buffer, def);
  JS_FreeCString(ctx, key);

  JSValue value = JS_NewArrayBufferCopy(ctx, p_buffer, ret);
  free(p_buffer);

  return value;
}

static const JSCFunctionListEntry prefs_funcs[] = {
    JSCFunctionListEntry{
        "begin", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, esp32_prefs_begin}
        }},
    JSCFunctionListEntry{
        "end", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, esp32_prefs_end}
        }},
    JSCFunctionListEntry{
        "isKey", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, esp32_prefs_isKey}
        }},
    JSCFunctionListEntry{
        "getType", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, esp32_prefs_getType}
        }},
    JSCFunctionListEntry{
        "remove", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, esp32_prefs_remove}
        }},
    JSCFunctionListEntry{
        "clear", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, esp32_prefs_clear}
        }},
    JSCFunctionListEntry{
        "putNumber", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, esp32_prefs_putNumber}
        }},
    JSCFunctionListEntry{
        "getNumber", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, esp32_prefs_getNumber}
        }},
    JSCFunctionListEntry{
        "putDouble", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, esp32_prefs_putDouble}
        }},
    JSCFunctionListEntry{
        "getDouble", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, esp32_prefs_getDouble}
        }},
    JSCFunctionListEntry{
        "putBool", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, esp32_prefs_putBool}
        }},
    JSCFunctionListEntry{
        "getBool", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, esp32_prefs_getBool}
        }},
    JSCFunctionListEntry{
        "putString", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, esp32_prefs_putString}
        }},
    JSCFunctionListEntry{
        "getString", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, esp32_prefs_getString}
        }},
    JSCFunctionListEntry{
        "putBytes", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, esp32_prefs_putBytes}
        }},
    JSCFunctionListEntry{
        "getBytes", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, esp32_prefs_getBytes}
        }},
};

JSModuleDef *addModule_prefs(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "Prefs", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, prefs_funcs,
                            sizeof(prefs_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, prefs_funcs,
        sizeof(prefs_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

void endModule_prefs(void)
{
  preferences.end();
}

JsModuleEntry prefs_module = {
  NULL,
  addModule_prefs,
  NULL,
  endModule_prefs
};