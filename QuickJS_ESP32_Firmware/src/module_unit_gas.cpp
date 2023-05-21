#include <Arduino.h>
#include "main_config.h"

#ifdef _UNIT_GAS_ENABLE_

#include "quickjs.h"
#include "module_unit_gas.h"
#include "Adafruit_SGP30.h"

static Adafruit_SGP30 sgp;

static JSValue unit_gas_begin(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  if( !sgp.begin() )
    return JS_EXCEPTION;

  return JS_UNDEFINED;
}

static JSValue unit_gas_iaqMeature(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  if( !sgp.IAQmeasure() )
    return JS_EXCEPTION;

  JSValue obj = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, obj, "TVOC", JS_NewUint32(ctx, sgp.TVOC));
  JS_SetPropertyStr(ctx, obj, "eCO2", JS_NewUint32(ctx, sgp.eCO2));
  return obj;
}

static const JSCFunctionListEntry unit_gas_funcs[] = {
    JSCFunctionListEntry{
        "begin", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, unit_gas_begin}
        }},
    JSCFunctionListEntry{
        "IAQmeasure", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, unit_gas_iaqMeature}
        }},
};

JSModuleDef *addModule_unit_gas(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "UnitGas", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, unit_gas_funcs,
                            sizeof(unit_gas_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, unit_gas_funcs,
        sizeof(unit_gas_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

JsModuleEntry unit_gas_module = {
  NULL,
  addModule_unit_gas,
  NULL,
  NULL
};

#endif
