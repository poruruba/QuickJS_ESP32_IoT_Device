#include <Arduino.h>
#include "main_config.h"

#ifdef _UNIT_SONICIO_ENABLE_

#include "quickjs.h"
#include "module_unit_sonicio.h"
#include "SONIC_IO.h"

static SONIC_IO sensor;

static JSValue unit_sonicio_begin(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  uint32_t trig, echo;
  JS_ToUint32(ctx, &trig, argv[0]);
  JS_ToUint32(ctx, &echo, argv[1]);

  sensor.begin(trig, echo);

  return JS_UNDEFINED;
}

static JSValue unit_sonicio_getDuration(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  float duration = sensor.getDuration();
  return JS_NewFloat64(ctx, duration);
}

static JSValue unit_sonicio_getDistance(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  float distance = sensor.getDistance();
  return JS_NewFloat64(ctx, distance);
}

static const JSCFunctionListEntry unit_sonicio_funcs[] = {
    JSCFunctionListEntry{
        "begin", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, unit_sonicio_begin}
        }},
    JSCFunctionListEntry{
        "getDuration", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, unit_sonicio_getDuration}
        }},
    JSCFunctionListEntry{
        "getDistance", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, unit_sonicio_getDistance}
        }},
};

JSModuleDef *addModule_unit_sonicio(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "UnitSonicIo", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, unit_sonicio_funcs,
                            sizeof(unit_sonicio_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, unit_sonicio_funcs,
        sizeof(unit_sonicio_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

JsModuleEntry unit_sonicio_module = {
  NULL,
  addModule_unit_sonicio,
  NULL,
  NULL
};

#endif
