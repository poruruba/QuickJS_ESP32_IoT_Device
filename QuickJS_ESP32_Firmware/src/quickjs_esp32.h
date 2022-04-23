#pragma once

#if defined(WiFi_h) && !defined(ENABLE_WIFI)
//#define ENABLE_WIFI
#endif

#include <Arduino.h>
#include <algorithm>
#include <vector>

#ifdef ENABLE_WIFI
#include <WiFi.h>
#include <HTTPClient.h>
#include <StreamString.h>
#endif

#include "quickjs.h"
#include "main_config.h"
#include "module_type.h"
#include "module_esp32.h"
#include "module_wire.h"
#include "module_input.h"
#include "module_gpio.h"
#include "module_utils.h"
#include "module_ledc.h"
#include "module_pixels.h"
#include "module_ir.h"
#include "module_udp.h"
#include "module_prefs.h"
#include "module_mqtt.h"
#include "module_env.h"
#ifdef _AUDIO_ENABLE_
#include "module_audio.h"
#endif
#ifdef _RTC_ENABLE_
#include "module_rtc.h"
#endif
#ifdef _IMU_ENABLE_
#include "module_imu.h"
#endif
#ifdef _LCD_ENABLE_
#include "module_lcd.h"
#endif
#ifdef _SD_ENABLE_
#include "module_sd.h"
#endif

static JsModuleEntry module_entries[] = {
  esp32_module,
  console_module,
  gpio_module,
  input_module,
  wire_module,
  wire1_module,
  pixels_module,
  utils_module,
  ledc_module,
  ir_module,
  udp_module,
  prefs_module,
  mqtt_module,
  env_module,
#ifdef _AUDIO_ENABLE_
  audio_module,
#endif
#ifdef _IMU_ENABLE_
  imu_module,
#endif 
#ifdef _LCD_ENABLE_
  lcd_module,
#endif 
#ifdef _RTC_ENABLE_
  rtc_module,
#endif
#ifdef _SD_ENABLE_
  sd_module
#endif
};

static void qjs_dump_exception(JSContext *ctx, JSValue v) {
  if (!JS_IsUndefined(v)) {
    const char *str = JS_ToCString(ctx, v);
    if (str) {
      Serial.println(str);
      JS_FreeCString(ctx, str);
    } else {
      Serial.println("[Exception]");
    }
  }
  JSValue e = JS_GetException(ctx);
  const char *str = JS_ToCString(ctx, e);
  if (str) {
    Serial.println(str);
    JS_FreeCString(ctx, str);
  }
  if (JS_IsError(ctx, e)) {
    JSValue s = JS_GetPropertyStr(ctx, e, "stack");
    if (!JS_IsUndefined(s)) {
      const char *str = JS_ToCString(ctx, s);
      if (str) {
        Serial.println(str);
        JS_FreeCString(ctx, str);
      }
    }
    JS_FreeValue(ctx, s);
  }
  JS_FreeValue(ctx, e);
}

#ifdef ENABLE_WIFI
class JSHttpFetcher {
  struct Entry {
    HTTPClient *client;
    JSValue resolving_funcs[2];
    int status;
    void result(JSContext *ctx, uint32_t func, JSValue body) {
      delete client;  // dispose connection before invoke;
      JSValue r = JS_NewObject(ctx);
      JS_SetPropertyStr(ctx, r, "body", JS_DupValue(ctx, body));
      JS_SetPropertyStr(ctx, r, "status", JS_NewInt32(ctx, status));
      JS_Call(ctx, resolving_funcs[func], JS_UNDEFINED, 1, &r);
      JS_FreeValue(ctx, r);
      JS_FreeValue(ctx, resolving_funcs[0]);
      JS_FreeValue(ctx, resolving_funcs[1]);
    }
  };
  std::vector<Entry *> queue;

 public:
  JSValue fetch(JSContext *ctx, JSValueConst jsUrl, JSValueConst options) {
    if (WiFi.status() != WL_CONNECTED) {
      return JS_EXCEPTION;
    }
    const char *url = JS_ToCString(ctx, jsUrl);
    if (!url) {
      return JS_EXCEPTION;
    }
    const char *method = nullptr, *body = nullptr;
    if (JS_IsObject(options)) {
      JSValue m = JS_GetPropertyStr(ctx, options, "method");
      if (JS_IsString(m)) {
        method = JS_ToCString(ctx, m);
      }
      JSValue b = JS_GetPropertyStr(ctx, options, "body");
      if (JS_IsString(m)) {
        body = JS_ToCString(ctx, b);
      }
    }

    Entry *ent = new Entry();
    ent->client = new HTTPClient();
    ent->client->begin(url);
    JS_FreeCString(ctx, url);

    // TODO: remove blocking calls.
    if (method) {
      ent->status = ent->client->sendRequest(method, (uint8_t *)body,
                                             body ? strlen(body) : 0);
    } else {
      ent->status = ent->client->GET();
    }
    queue.push_back(ent);

    JS_FreeCString(ctx, method);
    JS_FreeCString(ctx, body);
    return JS_NewPromiseCapability(ctx, ent->resolving_funcs);
  }

  void loop(JSContext *ctx) {
    int doneCount = 0;
    for (auto &pent : queue) {
      WiFiClient *stream = pent->client->getStreamPtr();
      if (stream == nullptr || pent->status <= 0) {
        // reject.
        pent->result(ctx, 1, JS_UNDEFINED);
        delete pent;
        pent = nullptr;
        doneCount++;
        continue;
      }
      if (stream->available()) {
        String body = pent->client->getString();
        JSValue bodyStr = JS_NewString(ctx, body.c_str());
        body.clear();
        pent->result(ctx, 0, bodyStr);
        JS_FreeValue(ctx, bodyStr);
        delete pent;
        pent = nullptr;
        doneCount++;
      }
    }

    if (doneCount > 0) {
      queue.erase(std::remove_if(queue.begin(), queue.end(),
                                 [](Entry *pent) { return pent == nullptr; }),
                  queue.end());
    }
  }
};
#endif  // ENABLE_WIFI

class JSTimer
{
  // 20 bytes / entry.
  struct TimerEntry {
    uint32_t id;
    int32_t timeout;
    int32_t interval;
    JSValue func;
  };
  std::vector<TimerEntry> timers;
  uint32_t id_counter = 0;

 public:
  uint32_t RegisterTimer(JSValue f, int32_t time, int32_t interval = -1) {
    uint32_t id = ++id_counter;
    timers.insert(timers.begin(), TimerEntry{id, time, interval, f});
    return id;
  }
  bool RemoveEntry(uint32_t id){
    auto newend = std::remove_if(timers.begin(), timers.end(),
                                [id](TimerEntry &t) {return t.id == id;});
    if( newend == timers.end() )
      return false;
    timers.erase(newend, timers.end());
    return true;
  }
  void RemoveTimer(JSContext *ctx, uint32_t id) {
    timers.erase(std::remove_if(timers.begin(), timers.end(),
                                [id, ctx](TimerEntry &t) {
                                    if(t.id == id){
                                      JS_FreeValue(ctx, t.func);
                                      return true;
                                    }else{
                                      return false;
                                    }
                                  }),
                 timers.end());
  }
  void forceTimeout(uint32_t id){
    std::vector<TimerEntry>::iterator a = std::find_if(timers.begin(), timers.end(), [id](TimerEntry &t)
                                             { return t.id == id; });
    if( a != timers.end() ){
      a->timeout = millis();
    }
  }

  void RemoveAll(JSContext *ctx){
    for (auto &ent : timers){
      JS_FreeValue(ctx, ent.func);
    }
    timers.clear();
  }  
  int32_t GetNextTimeout(int32_t now) {
    if (timers.empty()) {
      return -1;
    }
    std::sort(timers.begin(), timers.end(),
              [now](TimerEntry &a, TimerEntry &b) -> bool {
                return (a.timeout - now) >
                       (b.timeout - now);  // 2^32 wraparound
              });
    int next = timers.back().timeout - now;
    return max(next, 0);
  }
  bool ConsumeTimer(JSContext *ctx, int32_t now) {
    int32_t eps = 1;

    bool empty = true;
    while (!timers.empty() && timers.back().timeout - now < eps) {
      empty = false;
      auto ent = timers.back();

      // NOTE: may update timers in this JS_Call().
      JSValue r = JS_Call(ctx, ent.func, ent.func, 0, nullptr);
      if (JS_IsException(r)) {
        qjs_dump_exception(ctx, r);
      }
      JS_FreeValue(ctx, r);

      if (ent.interval >= 0) {
        ent.timeout = (ent.interval < 1) ? (now + 1) : (now + ent.interval);
        if( RemoveEntry(ent.id) )
          timers.insert(timers.begin(), ent);
      } else {
        RemoveTimer(ctx, ent.id);
      }
    }
    return !empty;
  }
};

class ESP32QuickJS {
 public:
  JSRuntime *rt = NULL;
  JSContext *ctx;
  JSTimer timer;
  JSValue loop_func = JS_UNDEFINED;
  JSValue btn_func[NUM_BTN_FUNC];

#ifdef ENABLE_WIFI
  JSHttpFetcher httpFetcher;
#endif

  ESP32QuickJS(){
    for( int i = 0 ; i < NUM_BTN_FUNC; i++ )
      btn_func[i] = JS_UNDEFINED;
  }

  void begin() {
    JSRuntime *rt = JS_NewRuntime();
    begin(rt, JS_NewContext(rt));
  }

  void begin(JSRuntime *rt, JSContext *ctx, int memoryLimit = 0) {
    this->rt = rt;
    this->ctx = ctx;
    if (memoryLimit == 0) {
      memoryLimit = ESP.getFreeHeap() >> 1;
    }
    JS_SetMemoryLimit(rt, memoryLimit);
    JS_SetGCThreshold(rt, memoryLimit >> 3);
    JSValue global = JS_GetGlobalObject(ctx);
    setup(ctx, global);
    JS_FreeValue(ctx, global);
  }

  void end() {
    if( rt == NULL )
      return;
      
    end_modules();
    
    if (JS_IsFunction(ctx, loop_func)){
      JS_FreeValue(ctx, loop_func);
      loop_func = JS_UNDEFINED;
    }
    for( uint8_t i = 0 ; i < NUM_BTN_FUNC ; i++ ){
      if (JS_IsFunction(ctx, btn_func[i])){
        JS_FreeValue(ctx, btn_func[i]);
        btn_func[i] = JS_UNDEFINED;
      }
    }

    timer.RemoveAll(ctx);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);

    rt = NULL;
  }

  JSValue callJsFunc(JSContext *ctx, JSValueConst func_obj, JSValueConst this_obj){
    JSValue ret = JS_Call(ctx, func_obj, this_obj, 0, nullptr);
    if (JS_IsException(ret)){
      qjs_dump_exception(ctx, ret);
    }

    return ret;
  }

  bool loop(bool callLoopFn = true)
  {
    if( rt == NULL )
      return false;
    if( !(g_fileloading == FILE_LOADING_NONE || g_fileloading == FILE_LOADING_TEXT) )
      return false;

    // async
    JSContext *c;
    int ret = JS_ExecutePendingJob(JS_GetRuntime(ctx), &c);
    if (ret < 0) {
      qjs_dump_exception(ctx, JS_UNDEFINED);
//      return false;
    }

    // timer
    uint32_t now = millis();
    if (timer.GetNextTimeout(now) >= 0) {
      timer.ConsumeTimer(ctx, now);
    }

#ifdef ENABLE_WIFI
        httpFetcher.loop(ctx);
#endif

    // loop()
    if( callLoopFn ){
      if (JS_IsFunction(ctx, loop_func)) {
      JSValue ret = JS_Call(ctx, loop_func, loop_func, 0, nullptr);
      if (JS_IsException(ret)) {
        qjs_dump_exception(ctx, ret);
        JS_FreeValue(ctx, ret);
//          return false;
        }else{
          JS_FreeValue(ctx, ret);
        }
      }

      for( int i = 0 ; i < NUM_BTN_FUNC ; i++ ){
        if( JS_IsFunction(ctx, btn_func[i]) && module_input_checkButtonState(FUNC_TYPE_WAS_PRESSED, i, 0) ){
          JSValue ret = JS_Call(ctx, btn_func[i], btn_func[i], 0, nullptr);
          if (JS_IsException(ret)) {
            qjs_dump_exception(ctx, ret);
            JS_FreeValue(ctx, ret);
//            return false;
      }else{
        JS_FreeValue(ctx, ret);
      }
    }
      }
    }

    return true;
  }

  void runGC() { JS_RunGC(rt); }

  bool exec(const char *code) {
    g_fileloading = FILE_LOADING_NONE;

    JSValue result = eval(code);
    bool ret = JS_IsException(result);
    JS_FreeValue(ctx, result);
    return ret;
  }

  JSValue eval(const char *code) {
    JSValue ret =
        JS_Eval(ctx, code, strlen(code), "<eval>", JS_EVAL_TYPE_MODULE);
    if (JS_IsException(ret)) {
      qjs_dump_exception(ctx, ret);
    }
    return ret;
  }

  // void setLoopFunc(const char *fname) {
  //   JSValue global = JS_GetGlobalObject(ctx);
  //   setLoopFunc(JS_GetPropertyStr(ctx, global, fname));
  //   JS_FreeValue(ctx, global);
  // }

  void setLoopFunc(JSValue f) {
    JS_FreeValue(ctx, loop_func);
    loop_func = f;
  }

  void setBtnFunc(JSValue f, uint8_t index) {
    if( index >= 0 && index < NUM_BTN_FUNC ){
      JS_FreeValue(ctx, btn_func[index]);
      btn_func[index] = f;
    }
  }

  int load_module(const void *buf, int buf_len, const char *filename) {
    int ret = 0;

    /* for the modules, we compile then run to be able to set import.meta */
    JSValue val = JS_Eval(this->ctx, (const char*)buf, buf_len, filename,
                  JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY);
    if (!JS_IsException(val)) {
//              js_module_set_import_meta(ctx, val, TRUE, TRUE);
      val = JS_EvalFunction(this->ctx, val);
    }
    if (JS_IsException(val)) {
      qjs_dump_exception(this->ctx, val);
      ret = -1;
    }
    JS_FreeValue(this->ctx, val);

    // JSMemoryUsage usage;
    // JS_ComputeMemoryUsage(this->rt, &usage);
    // Serial.printf("malloc_size=%ld, malloc_limit=%ld, memory_usage_size=%ld\n", usage.malloc_size, usage.malloc_limit, usage.memory_used_size);

    // uint32_t size = ESP.getFreeHeap();
    // Serial.printf("FreeHeap=%d\n", size);

    return ret;
  }

  void getMemoryUsage(JSMemoryUsage *usage){
    JS_ComputeMemoryUsage(this->rt, usage);
  }

  void initialize_modules(void){
    int num = sizeof(module_entries) / sizeof(JsModuleEntry);
    for( int i = 0 ; i < num ; i++ ){
      if( module_entries[i].initializeImpl != NULL )
        module_entries[i].initializeImpl();
    }
  }

  void add_modules(JSValue global){
    if( rt == NULL )
      return; 
         
    int num = sizeof(module_entries) / sizeof(JsModuleEntry);
    for( int i = 0 ; i < num ; i++ ){
      if( module_entries[i].addImpl != NULL )
        module_entries[i].addImpl(ctx, global);
    }
  }

  void update_modules(void) {
    if( rt == NULL )
      return; 

    int num = sizeof(module_entries) / sizeof(JsModuleEntry);
    for( int i = 0 ; i < num ; i++ ){
      if( module_entries[i].loopImpl != NULL )
        module_entries[i].loopImpl();
    }
  }

  void end_modules(void){
    if( rt == NULL )
      return; 
         
    int num = sizeof(module_entries) / sizeof(JsModuleEntry);
    for( int i = 0 ; i < num ; i++ ){
      if( module_entries[i].endImpl != NULL )
        module_entries[i].endImpl();
    }
  }

protected:

  virtual void setup(JSContext *ctx, JSValue global) {
    this->ctx = ctx;
    JS_SetContextOpaque(ctx, this);

//  setup console.log()
//  JSValue console = JS_NewObject(ctx);
//  JS_SetPropertyStr(ctx, global, "console", console);
//  JS_SetPropertyStr(ctx, console, "log", JS_NewCFunction(ctx, console_log, "log", 1));

    // timer
    JS_SetPropertyStr(ctx, global, "setTimeout", JS_NewCFunction(ctx, set_timeout, "setTimeout", 2));
    JS_SetPropertyStr(ctx, global, "clearTimeout", JS_NewCFunction(ctx, clear_timeout, "clearTimeout", 1));
    JS_SetPropertyStr(ctx, global, "setInterval", JS_NewCFunction(ctx, set_interval, "setInterval", 2));
    JS_SetPropertyStr(ctx, global, "clearInterval",  JS_NewCFunction(ctx, clear_timeout, "clearInterval", 1));

    JS_SetPropertyStr(ctx, global, "millis", JS_NewCFunction(ctx, esp32_millis, "millis", 0));
    JS_SetPropertyStr(ctx, global, "delay", JS_NewCFunction(ctx, esp32_delay, "delay", 1));
    JS_SetPropertyStr(ctx, global, "randomSeed", JS_NewCFunction(ctx, esp32_randomSeed, "randomSeed", 1));
    JS_SetPropertyStr(ctx, global, "random", JS_NewCFunction(ctx, esp32_random, "random", 2));

#ifdef ENABLE_WIFI
    JS_SetPropertyStr(ctx, global, "isWifiConnected", JS_NewCFunction(ctx, wifi_is_connected, "isWifiConnected", 0));
    JS_SetPropertyStr(ctx, global, "fetch", JS_NewCFunction(ctx, http_fetch, "fetch", 2));

    // JSCFunctionListEntry{"isWifiConnected", 0, JS_DEF_CFUNC, 0, {
    //                        func : {0, JS_CFUNC_generic, wifi_is_connected}
    //                      }},
    // JSCFunctionListEntry{"fetch", 0, JS_DEF_CFUNC, 0, {
    //                        func : {2, JS_CFUNC_generic, http_fetch}
    //                      }},
#endif

    add_modules(global);
  }

  // static JSValue console_log(JSContext *ctx, JSValueConst jsThis, int argc,
  //                            JSValueConst *argv) {
  //   for (int i = 0; i < argc; i++) {
  //     const char *str = JS_ToCString(ctx, argv[i]);
  //     if (str) {
  //       Serial.println(str);
  //       JS_FreeCString(ctx, str);
  //     }
  //   }
  //   return JS_UNDEFINED;
  // }

  static JSValue set_timeout(JSContext *ctx, JSValueConst jsThis, int argc,
                             JSValueConst *argv) {
    ESP32QuickJS *qjs = (ESP32QuickJS *)JS_GetContextOpaque(ctx);
    uint32_t t;
    JS_ToUint32(ctx, &t, argv[1]);
    uint32_t id =
        qjs->timer.RegisterTimer(JS_DupValue(ctx, argv[0]), millis() + t);
    return JS_NewUint32(ctx, id);
  }

  static JSValue clear_timeout(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv) {
    ESP32QuickJS *qjs = (ESP32QuickJS *)JS_GetContextOpaque(ctx);
    uint32_t tid;
    JS_ToUint32(ctx, &tid, argv[0]);
    qjs->timer.RemoveTimer(ctx, tid);
    return JS_UNDEFINED;
  }

  static JSValue set_interval(JSContext *ctx, JSValueConst jsThis, int argc,
                              JSValueConst *argv) {
    ESP32QuickJS *qjs = (ESP32QuickJS *)JS_GetContextOpaque(ctx);
    uint32_t t;
    JS_ToUint32(ctx, &t, argv[1]);
    uint32_t id =
        qjs->timer.RegisterTimer(JS_DupValue(ctx, argv[0]), millis() + t, t);
    return JS_NewUint32(ctx, id);
  }

  static JSValue esp32_millis(JSContext *ctx, JSValueConst jsThis, int argc,
                              JSValueConst *argv) {
    return JS_NewUint32(ctx, millis());
  }

  static JSValue esp32_delay(JSContext *ctx, JSValueConst jsThis, int argc,
                              JSValueConst *argv){
    uint32_t value;
    JS_ToUint32(ctx, &value, argv[0]);
    delay(value);
    return JS_UNDEFINED;
  }

  static JSValue esp32_randomSeed(JSContext *ctx, JSValueConst jsThis, int argc,
                              JSValueConst *argv){
    int32_t seed;
    JS_ToInt32(ctx, &seed, argv[0]);

    randomSeed(seed);

    return JS_UNDEFINED;
  }

  static JSValue esp32_random(JSContext *ctx, JSValueConst jsThis, int argc,
                              JSValueConst *argv){
  long ret;
  if( argc <= 1 ){
    int32_t max;
    JS_ToInt32(ctx, &max, argv[0]);

    ret = random(max);
  }else{
    int32_t min, max;
    JS_ToInt32(ctx, &min, argv[0]);
    JS_ToInt32(ctx, &max, argv[1]);

    ret = random(min, max);
  }

  return JS_NewInt32(ctx, ret);
}

#ifdef ENABLE_WIFI
  static JSValue wifi_is_connected(JSContext *ctx, JSValueConst jsThis,
                                   int argc, JSValueConst *argv) {
    return JS_NewBool(ctx, WiFi.status() == WL_CONNECTED);
  }

  static JSValue http_fetch(JSContext *ctx, JSValueConst jsThis, int argc,
                            JSValueConst *argv) {
    ESP32QuickJS *qjs = (ESP32QuickJS *)JS_GetContextOpaque(ctx);
    return qjs->httpFetcher.fetch(ctx, argv[0], argv[1]);
  }
#endif
};
