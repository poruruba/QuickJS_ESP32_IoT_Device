#include <Arduino.h>
#include "main_config.h"

#ifdef _RTC_ENABLE_

#include "module_type.h"
#include "quickjs.h"

static JSValue esp32_rtc_SetTime(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  uint32_t hours, minutes, seconds;
  JS_ToUint32(ctx, &hours, argv[0]);
  JS_ToUint32(ctx, &minutes, argv[1]);
  JS_ToUint32(ctx, &seconds, argv[2]);
  RTC_TimeTypeDef def;
  def.Hours = hours;
  def.Minutes = minutes;
  def.Seconds = seconds;
  M5.Rtc.SetTime(&def);
  return JS_UNDEFINED;
}

static JSValue esp32_rtc_SetDate(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  uint32_t year, month, date, weekday;
  JS_ToUint32(ctx, &year, argv[0]);
  JS_ToUint32(ctx, &month, argv[1]);
  JS_ToUint32(ctx, &date, argv[2]);
  JS_ToUint32(ctx, &weekday, argv[3]);
  RTC_DateTypeDef def;
  def.Year = year;
  def.Month = month;
  def.Date = date;
  def.WeekDay = weekday;
#if defined(ARDUINO_M5Stick_C)
  M5.Rtc.SetData(&def);
#elif defined(ARDUINO_M5STACK_Core2)
  M5.Rtc.SetDate(&def);
#endif
  return JS_UNDEFINED;
}

static JSValue esp32_rtc_GetTime(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  RTC_TimeTypeDef def;
  M5.Rtc.GetTime(&def);
  JSValue obj = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, obj, "Hours", JS_NewUint32(ctx, def.Hours));
  JS_SetPropertyStr(ctx, obj, "Minutes", JS_NewUint32(ctx, def.Minutes));
  JS_SetPropertyStr(ctx, obj, "Seconds", JS_NewUint32(ctx, def.Seconds));
  return obj;
}

static JSValue esp32_rtc_GetDate(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  RTC_DateTypeDef def;
#if defined(ARDUINO_M5Stick_C)
  M5.Rtc.GetData(&def);
#elif defined(ARDUINO_M5STACK_Core2)
  M5.Rtc.GetDate(&def);
#endif
  JSValue obj = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, obj, "Year", JS_NewUint32(ctx, def.Year));
  JS_SetPropertyStr(ctx, obj, "Month", JS_NewUint32(ctx, def.Month));
  JS_SetPropertyStr(ctx, obj, "Date", JS_NewUint32(ctx, def.Date));
  JS_SetPropertyStr(ctx, obj, "WeekDay", JS_NewUint32(ctx, def.WeekDay));
  return obj;
}

static const JSCFunctionListEntry rtc_funcs[] = {
    JSCFunctionListEntry{
        "setTime", 0, JS_DEF_CFUNC, 0, {
          func : {3, JS_CFUNC_generic, esp32_rtc_SetTime}
        }},
    JSCFunctionListEntry{
        "setDate", 0, JS_DEF_CFUNC, 0, {
          func : {4, JS_CFUNC_generic, esp32_rtc_SetDate}
        }},
    JSCFunctionListEntry{
        "getTime", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, esp32_rtc_GetTime}
        }},
    JSCFunctionListEntry{
        "getDate", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, esp32_rtc_GetDate}
        }},
};

JSModuleDef *addModule_rtc(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;

  mod = JS_NewCModule(ctx, "Rtc", [](JSContext *ctx, JSModuleDef *m) {
          return JS_SetModuleExportList(
              ctx, m, rtc_funcs,
              sizeof(rtc_funcs) / sizeof(JSCFunctionListEntry));
        });
  if (mod) {
    JS_AddModuleExportList(
        ctx, mod, rtc_funcs,
        sizeof(rtc_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

long initialize_rtc(void){
  M5.Rtc.begin();

  configTzTime("JST-9", "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
  struct tm timeInfo;
  getLocalTime(&timeInfo);

  RTC_DateTypeDef def;
  def.Year = timeInfo.tm_year + 1900;
  def.Month = timeInfo.tm_mon + 1;
  def.Date = timeInfo.tm_mday;
  def.WeekDay = timeInfo.tm_wday;
#if defined(ARDUINO_M5Stick_C)
  M5.Rtc.SetData(&def);
#elif defined(ARDUINO_M5STACK_Core2)
  M5.Rtc.SetDate(&def);
#endif

  RTC_TimeTypeDef def2;
  def2.Hours = timeInfo.tm_hour;
  def2.Minutes = timeInfo.tm_min;
  def2.Seconds = timeInfo.tm_sec;
  M5.Rtc.SetTime(&def2);

  return 0;
}

JsModuleEntry rtc_module = {
  initialize_rtc,
  addModule_rtc,
  NULL,
  NULL
};
#endif
