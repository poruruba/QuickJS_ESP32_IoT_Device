#include <Arduino.h>
#include "main_config.h"

#ifdef _UNIT_GESTURE_ENABLE_

#include "quickjs.h"
#include "module_unit_gesture.h"
#include <DFRobot_PAJ7620U2.h>

static DFRobot_PAJ7620U2 sensor;

static JSValue unit_gesture_begin(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  if( sensor.begin() != ERR_OK )
    return JS_EXCEPTION;

  sensor.setGestureHighRate(true);

  return JS_UNDEFINED;
}

static JSValue unit_gesture_setGestureHighRate(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  bool enable = JS_ToBool(ctx, argv[0]);
  sensor.setGestureHighRate(enable); 

  return JS_UNDEFINED;
}

static JSValue unit_gesture_getGesture(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  DFRobot_PAJ7620U2::eGesture_t gesture = sensor.getGesture(); 

  return JS_NewUint32(ctx, (uint32_t)gesture);
}

static JSValue unit_gesture_getDescription(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t gesture;
  JS_ToUint32(ctx, &gesture, argv[0]);
  String description = sensor.gestureDescription((DFRobot_PAJ7620U2::eGesture_t)gesture); 

  return JS_NewString(ctx, description.c_str());
}

static const JSCFunctionListEntry unit_gesture_funcs[] = {
    JSCFunctionListEntry{
        "begin", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, unit_gesture_begin}
        }},
    JSCFunctionListEntry{
        "setGestureHighRate", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, unit_gesture_setGestureHighRate}
        }},
    JSCFunctionListEntry{
        "getGesture", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, unit_gesture_getGesture}
        }},
    JSCFunctionListEntry{
        "getDescription", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, unit_gesture_getDescription}
        }},
    JSCFunctionListEntry{
        "None", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureNone
        }},
    JSCFunctionListEntry{
        "Right", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureRight
        }},
    JSCFunctionListEntry{
        "Left", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureLeft
        }},
    JSCFunctionListEntry{
        "Up", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureUp
        }},
    JSCFunctionListEntry{
        "Down", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureDown
        }},
    JSCFunctionListEntry{
        "Forward", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureForward
        }},
    JSCFunctionListEntry{
        "Backward", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureBackward
        }},
    JSCFunctionListEntry{
        "Clockwise", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureClockwise
        }},
    JSCFunctionListEntry{
        "AntiClockwise", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureAntiClockwise
        }},
    JSCFunctionListEntry{
        "Wave", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureWave
        }},
    JSCFunctionListEntry{
        "WaveSlowlyDisorder", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureWaveSlowlyDisorder
        }},
    JSCFunctionListEntry{
        "WaveSlowlyLeftRight", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureWaveSlowlyLeftRight
        }},
    JSCFunctionListEntry{
        "WaveSlowlyUpDown", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureWaveSlowlyUpDown
        }},
    JSCFunctionListEntry{
        "WaveSlowlyForwardBackward", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureWaveSlowlyForwardBackward
        }},
    JSCFunctionListEntry{
        "All", 0, JS_DEF_PROP_INT32, 0, {
          i32 : DFRobot_PAJ7620U2::eGestureAll
        }},
};

JSModuleDef *addModule_unit_gesture(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "UnitGesture", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, unit_gesture_funcs,
                            sizeof(unit_gesture_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, unit_gesture_funcs,
        sizeof(unit_gesture_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

JsModuleEntry unit_gesture_module = {
  NULL,
  addModule_unit_gesture,
  NULL,
  NULL
};

#endif
