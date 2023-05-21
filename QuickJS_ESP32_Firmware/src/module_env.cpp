#include <Arduino.h>
#include "main_config.h"

#ifdef _ENV_ENABLE_

#include "quickjs.h"
#include "module_env.h"

#include "SHT3X.h"
#include "DHT12.h"

static DHT12 dht12;
static SHT3X sht30;

static JSValue env_dht12_readTemperature(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  float tmp = dht12.readTemperature();
  return JS_NewFloat64(ctx, tmp);
}

static JSValue env_dht12_readHumidity(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  float hum = dht12.readHumidity();
  return JS_NewFloat64(ctx, hum);
}

static JSValue env_sht30_get(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  if(sht30.get() != 0)
    return JS_EXCEPTION;
  
  JSValue obj = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, obj, "cTemp", JS_NewFloat64(ctx, sht30.cTemp));
  JS_SetPropertyStr(ctx, obj, "fTemp", JS_NewFloat64(ctx, sht30.fTemp));
  JS_SetPropertyStr(ctx, obj, "humidity", JS_NewFloat64(ctx, sht30.humidity));
  return obj;
}

static const JSCFunctionListEntry env_funcs[] = {
    JSCFunctionListEntry{
        "dht12_readTemperature", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, env_dht12_readTemperature}
        }},
    JSCFunctionListEntry{
        "dht12_readHumidity", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, env_dht12_readHumidity}
        }},
    JSCFunctionListEntry{
        "sht30_get", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, env_sht30_get}
        }},
};

JSModuleDef *addModule_env(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "Env", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, env_funcs,
                            sizeof(env_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, env_funcs,
        sizeof(env_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

JsModuleEntry env_module = {
  NULL,
  addModule_env,
  NULL,
  NULL
};

#endif
