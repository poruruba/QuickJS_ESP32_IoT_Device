#include <Arduino.h>
#include "main_config.h"
#include "quickjs.h"
#include "module_imu.h"

#ifdef _IMU_ENABLE_

static JSValue esp32_imu_getAccelData(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  JSValue obj = JS_NewObject(ctx);
  float ax, ay, az;
  M5.IMU.getAccelData(&ax, &ay, &az);
  JS_SetPropertyStr(ctx, obj, "x", JS_NewFloat64(ctx, ax));
  JS_SetPropertyStr(ctx, obj, "y", JS_NewFloat64(ctx, ay));
  JS_SetPropertyStr(ctx, obj, "z", JS_NewFloat64(ctx, az));
  return obj;
}

static JSValue esp32_imu_getGyroData(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  JSValue obj = JS_NewObject(ctx);
  float gx, gy, gz;
  M5.IMU.getGyroData(&gx, &gy, &gz);
  JS_SetPropertyStr(ctx, obj, "x", JS_NewFloat64(ctx, gx));
  JS_SetPropertyStr(ctx, obj, "y", JS_NewFloat64(ctx, gy));
  JS_SetPropertyStr(ctx, obj, "z", JS_NewFloat64(ctx, gz));
  return obj;
}

static JSValue esp32_imu_getTempData(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  float t;
  M5.IMU.getTempData(&t);
  return JS_NewFloat64(ctx, t);
}

static const JSCFunctionListEntry imu_funcs[] = {
    JSCFunctionListEntry{
        "getAccelData", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, esp32_imu_getAccelData}
        }},
    JSCFunctionListEntry{
        "getGyroData", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, esp32_imu_getGyroData}
        }},
    JSCFunctionListEntry{
        "getTempData", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, esp32_imu_getTempData}
        }},
};

JSModuleDef *addModule_imu(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "Imu", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, imu_funcs,
                            sizeof(imu_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, imu_funcs,
        sizeof(imu_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

long initialize_imu(void){
  M5.IMU.Init();

  return 0;
}

JsModuleEntry imu_module = {
  initialize_imu,
  addModule_imu,
  NULL,
  NULL
};

#endif
