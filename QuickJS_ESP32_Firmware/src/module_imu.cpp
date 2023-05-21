#include <Arduino.h>
#include "main_config.h"

#ifdef _IMU_ENABLE_

#include "quickjs.h"
#include "module_imu.h"

static JSValue esp32_imu_getAccelData(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  JSValue obj = JS_NewObject(ctx);
  float ax, ay, az;
#if defined(ARDUINO_ESP32S3_DEV)
  M5.IMU.getAccel(&ax, &ay, &az);
#else
  M5.IMU.getAccelData(&ax, &ay, &az);
#endif
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
#if defined(ARDUINO_ESP32S3_DEV)
  M5.IMU.getGyro(&gx, &gy, &gz);
#else
  M5.IMU.getGyroData(&gx, &gy, &gz);
#endif
  JS_SetPropertyStr(ctx, obj, "x", JS_NewFloat64(ctx, gx));
  JS_SetPropertyStr(ctx, obj, "y", JS_NewFloat64(ctx, gy));
  JS_SetPropertyStr(ctx, obj, "z", JS_NewFloat64(ctx, gz));
  return obj;
}

static JSValue esp32_imu_getTempData(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  float t;
#if defined(ARDUINO_ESP32S3_DEV)
  M5.IMU.getTemp(&t);
#else
  M5.IMU.getTempData(&t);
#endif
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
#if defined(ARDUINO_ESP32S3_DEV)
  M5.IMU.begin();
#else
  M5.IMU.Init();
#endif

  return 0;
}

JsModuleEntry imu_module = {
  initialize_imu,
  addModule_imu,
  NULL,
  NULL
};

#endif
