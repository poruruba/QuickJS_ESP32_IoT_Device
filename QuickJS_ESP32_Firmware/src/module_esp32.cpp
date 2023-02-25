#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Syslog.h>

#include "main_config.h"
#include "quickjs.h"
#include "quickjs_esp32.h"
#include "module_type.h"
#include "module_esp32.h"

#define GLOBAL_ESP32
#define GLOBAL_CONSOLE

#define MODEL_OTHER         0
#define MODEL_M5Core2       1
#define MODEL_M5Core        2
#define MODEL_M5Fire        3
#define MODEL_M5StickCPlus  4
#define MODEL_M5CoreInk     5
#define MODEL_M5Paper       6
#define MODEL_M5Tough       7
#define MODEL_M5StickC      8
#define MODEL_M5Atom        9
#define MODEL_M5StampC3     10
#define MODEL_M5StampC3U    11

static WiFiUDP syslog_udp;
static Syslog g_syslog(syslog_udp);
static char *p_syslog_host = NULL;

long syslog_send(const char *p_message)
{
  bool ret = g_syslog.log(LOG_INFO, p_message);
  return ret ? 0 : -1;
}

long syslog_changeServer(const char *host, uint16_t port)
{
  g_syslog.appName(MDNS_SERVICE);
  g_syslog.deviceHostname(MDNS_NAME);
  g_syslog.defaultPriority(LOG_INFO | LOG_USER);

  if( p_syslog_host != NULL )
    free(p_syslog_host);
  p_syslog_host = (char*)malloc(strlen(host) + 1);
  if( p_syslog_host == NULL )
    return -1;
  strcpy(p_syslog_host, host);
  g_syslog.server(p_syslog_host, port);

  Serial.printf("syslog: host=%s, port=%d\n", p_syslog_host, port);

  return 0;
}

static JSValue esp32_reboot(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  g_download_buffer[0] = '\0';
  g_fileloading = FILE_LOADING_REBOOT;
  
  return JS_UNDEFINED;
}

static JSValue esp32_restart(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  g_download_buffer[0] = '\0';
  g_fileloading = FILE_LOADING_RESTART;

  return JS_UNDEFINED;
}

static JSValue esp32_set_loop(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  ESP32QuickJS *qjs = (ESP32QuickJS *)JS_GetContextOpaque(ctx);
  qjs->setLoopFunc(JS_DupValue(ctx, argv[0]));
  return JS_UNDEFINED;
}

static JSValue esp32_update(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  ESP32QuickJS *qjs = (ESP32QuickJS *)JS_GetContextOpaque(ctx);
  qjs->update_modules();
  return JS_UNDEFINED;
}

static JSValue esp32_get_ipaddress(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  IPAddress address = WiFi.localIP();  
  return JS_NewUint32(ctx, (uint32_t)(((uint32_t)address[0]) << 24 | address[1] << 16 | address[2] << 8 | address[3]));
}

static JSValue esp32_get_macaddress(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  uint8_t address[6];
  WiFi.macAddress(address);

  JSValue jsArray = JS_NewArray(ctx);
  for (int i = 0; i < 6; i++)
    JS_SetPropertyUint32(ctx, jsArray, i, JS_NewInt32(ctx, address[i]));

  return jsArray;
}

static JSValue esp32_get_deviceModel(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  uint8_t model = MODEL_OTHER;
#if defined(ARDUINO_M5Stack_Core_ESP32) // M5StackCore
  model = MODEL_M5Core;
#elif defined(ARDUINO_M5STACK_FIRE)  //M5Stack Fire
  model = MODEL_M5Fire;
#elif defined(ARDUINO_M5STACK_Core2) // M5Stack Core2
  model = MODEL_M5Core2;
#elif defined(ARDUINO_M5Stick_C) // M5Stick C
  model = MODEL_M5StickC;
#elif defined(ARDUINO_M5Stick_C_Plus) // M5StickCPlus
  model = MODEL_M5StickCPlus;
#elif defined(ARDUINO_M5Stack_CoreInk) // M5Stack CoreInk
  model = MODEL_M5CoreInk;
#elif defined(ARDUINO_M5STACK_Paper) // M5Paper
  model = MODEL_M5Paper;
#elif defined(ARDUINO_M5STACK_TOUGH) // M5Tough
  model = MODEL_M5Tough;
#elif defined(ARDUINO_M5Stack_ATOM) // M5Atom
  model = MODEL_M5Atom;
#elif defined(ARDUINO_ESP32C3_DEV) // M5StampC3
  model = MODEL_M5StampC3;
#elif defined(ARDUINO_ESP32C3U_DEV) // M5StampC3U
  model = MODEL_M5StampC3U;
#endif
  return JS_NewUint32(ctx, model);
}

static JSValue esp32_check_putText(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  if( g_fileloading != FILE_LOADING_TEXT ){
    return JS_NULL;
  }

  JSValue value = JS_NewString(ctx, g_download_buffer);
  g_fileloading = FILE_LOADING_NONE;

  return value;
}

static JSValue esp32_syslog(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *message = JS_ToCString(ctx, argv[0]);
  if( message == NULL )
    return JS_EXCEPTION;

  syslog_send(message);

  JS_FreeCString(ctx, message);

  return JS_UNDEFINED;
}

static JSValue esp32_setSyslogServer(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *host = JS_ToCString(ctx, argv[0]);
  if( host == NULL )
    return JS_EXCEPTION;
  uint32_t port;
  JS_ToUint32(ctx, &port, argv[1]);

  String server(host);
  server += ":";
  server += String(port);

  long ret;  
  ret = write_config_string(CONFIG_FNAME_SYSLOG, server.c_str());
  if( ret != 0 ){
    JS_FreeCString(ctx, host);
    return JS_EXCEPTION;
  }

  ret = syslog_changeServer(host, port);
  JS_FreeCString(ctx, host);
  if( ret != 0 ){
    return JS_EXCEPTION;
  }

  return JS_UNDEFINED;
}

static JSValue esp32_getSyslogServer(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  String server = read_config_string(CONFIG_FNAME_SYSLOG);
  int delim = server.indexOf(':');
  if( delim < 0 )
    return JS_EXCEPTION;
  String host = server.substring(0, delim);
  String port = server.substring(delim + 1);

  JSValue obj = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, obj, "host", JS_NewString(ctx, host.c_str()));
  JS_SetPropertyStr(ctx, obj, "port", JS_NewUint32(ctx, port.toInt()));
  
  return obj;
}

static JSValue esp32_getMemoryUsage(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  JSMemoryUsage usage;
  ESP32QuickJS *qjs = (ESP32QuickJS *)JS_GetContextOpaque(ctx);
  qjs->getMemoryUsage(&usage);

  JSValue obj = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, obj, "malloc_limit", JS_NewUint32(ctx, usage.malloc_limit));
  JS_SetPropertyStr(ctx, obj, "memory_usage_size", JS_NewUint32(ctx, usage.memory_used_size));
  JS_SetPropertyStr(ctx, obj, "malloc_size", JS_NewUint32(ctx, usage.malloc_size));
  JS_SetPropertyStr(ctx, obj, "free_heap", JS_NewUint32(ctx, ESP.getFreeHeap()));

  return obj;
}

static JSValue esp32_console_log(JSContext *ctx, JSValueConst jsThis, int argc,
                            JSValueConst *argv, int magic) {
  int i = 0;
  if( magic == 0 ){
    bool enable = JS_ToBool(ctx, argv[0]);
    if( enable )
      return JS_UNDEFINED;
    i = 1;
  }
  for (; i < argc; i++) {
    const char *str = JS_ToCString(ctx, argv[i]);
    if (str) {
      if( magic == 2 ) Serial.print("[info]");
      else if( magic == 3 ) Serial.print("[debug] ");
      else if( magic == 4 ) Serial.print("[warn] ");
      else if( magic == 5 ) Serial.print("[error] ");
      Serial.println(str);
      JS_FreeCString(ctx, str);
    }
  }

  return JS_UNDEFINED;
}

static const JSCFunctionListEntry console_funcs[] = {
    JSCFunctionListEntry{"assert", 0, JS_DEF_CFUNC, 0, {
                           func : {2, JS_CFUNC_generic_magic, {generic_magic : esp32_console_log }}
                         }},
    JSCFunctionListEntry{"log", 0, JS_DEF_CFUNC, 1, {
                           func : {1, JS_CFUNC_generic_magic, {generic_magic : esp32_console_log }}
                         }},
    JSCFunctionListEntry{"info", 0, JS_DEF_CFUNC, 2, {
                           func : {1, JS_CFUNC_generic_magic, {generic_magic : esp32_console_log }}
                         }},
    JSCFunctionListEntry{"debug", 0, JS_DEF_CFUNC, 3, {
                           func : {1, JS_CFUNC_generic_magic, {generic_magic : esp32_console_log }}
                         }},
    JSCFunctionListEntry{"warn", 0, JS_DEF_CFUNC, 4, {
                           func : {1, JS_CFUNC_generic_magic, {generic_magic : esp32_console_log }}
                         }},
    JSCFunctionListEntry{"error", 0, JS_DEF_CFUNC, 5, {
                           func : {1, JS_CFUNC_generic_magic, {generic_magic : esp32_console_log }}
                         }},
};

JSModuleDef *addModule_console(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "Console", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, console_funcs,
                            sizeof(console_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, console_funcs,
        sizeof(console_funcs) / sizeof(JSCFunctionListEntry));
  }

#ifdef GLOBAL_CONSOLE
  // import * as console from "Console";
  JSValue console = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, global, "console", console);
  JS_SetPropertyFunctionList(
      ctx, console, console_funcs,
      sizeof(console_funcs) / sizeof(JSCFunctionListEntry));
#endif

  return mod;
}

static const JSCFunctionListEntry esp32_funcs[] = {
    JSCFunctionListEntry{"reboot", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, esp32_reboot}
                         }},
    JSCFunctionListEntry{"restart", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, esp32_restart}
                         }},
    JSCFunctionListEntry{"setLoop", 0, JS_DEF_CFUNC, 0, {
                           func : {1, JS_CFUNC_generic, esp32_set_loop}
                         }},
    JSCFunctionListEntry{"update", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, esp32_update}
                         }},
    JSCFunctionListEntry{"getIpAddress", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, esp32_get_ipaddress}
                         }},
    JSCFunctionListEntry{"getMacAddress", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, esp32_get_macaddress}
                         }},
    JSCFunctionListEntry{"getDeviceModel", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, esp32_get_deviceModel}
                         }},
    JSCFunctionListEntry{"checkPutText", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, esp32_check_putText}
                         }},
    JSCFunctionListEntry{"syslog", 0, JS_DEF_CFUNC, 0, {
                           func : {1, JS_CFUNC_generic, esp32_syslog}
                         }},
    JSCFunctionListEntry{"setSyslogServer", 0, JS_DEF_CFUNC, 0, {
                           func : {2, JS_CFUNC_generic, esp32_setSyslogServer}
                         }},
    JSCFunctionListEntry{"getSyslogServer", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, esp32_getSyslogServer}
                         }},
    JSCFunctionListEntry{"getMemoryUsage", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, esp32_getMemoryUsage}
                         }},
    JSCFunctionListEntry{
        "MODEL_OTHER", 0, JS_DEF_PROP_INT32, 0, {
          i32 : MODEL_OTHER
        }},
    JSCFunctionListEntry{
        "MODEL_M5Core2", 0, JS_DEF_PROP_INT32, 0, {
          i32 : MODEL_M5Core2
        }},
    JSCFunctionListEntry{
        "MODEL_M5Core", 0, JS_DEF_PROP_INT32, 0, {
          i32 : MODEL_M5Core
        }},
    JSCFunctionListEntry{
        "MODEL_M5Fire", 0, JS_DEF_PROP_INT32, 0, {
          i32 : MODEL_M5Fire
        }},
    JSCFunctionListEntry{
        "MODEL_M5StickCPlus", 0, JS_DEF_PROP_INT32, 0, {
          i32 : MODEL_M5StickCPlus
        }},
    JSCFunctionListEntry{
        "MODEL_M5CoreInk", 0, JS_DEF_PROP_INT32, 0, {
          i32 : MODEL_M5CoreInk
        }},
    JSCFunctionListEntry{
        "MODEL_M5Paper", 0, JS_DEF_PROP_INT32, 0, {
          i32 : MODEL_M5Paper
        }},
    JSCFunctionListEntry{
        "MODEL_M5Tough", 0, JS_DEF_PROP_INT32, 0, {
          i32 : MODEL_M5Tough
        }},
    JSCFunctionListEntry{
        "MODEL_M5StickC", 0, JS_DEF_PROP_INT32, 0, {
          i32 : MODEL_M5StickC
        }},
    JSCFunctionListEntry{
        "MODEL_M5Atom", 0, JS_DEF_PROP_INT32, 0, {
          i32 : MODEL_M5Atom
        }},
    JSCFunctionListEntry{
        "MODEL_M5StampC3", 0, JS_DEF_PROP_INT32, 0, {
          i32 : MODEL_M5StampC3
        }},
    JSCFunctionListEntry{
        "MODEL_M5StampC3U", 0, JS_DEF_PROP_INT32, 0, {
          i32 : MODEL_M5StampC3U
        }},
};

JSModuleDef *addModule_esp32(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "Esp32", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, esp32_funcs,
                            sizeof(esp32_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, esp32_funcs,
        sizeof(esp32_funcs) / sizeof(JSCFunctionListEntry));
  }

#ifdef GLOBAL_ESP32
  // import * as esp32 from "Esp32";
  JSValue esp32 = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, global, "esp32", esp32);
  JS_SetPropertyFunctionList(
      ctx, esp32, esp32_funcs,
      sizeof(esp32_funcs) / sizeof(JSCFunctionListEntry));
#endif

  return mod;
}

long initialize_esp32(void)
{
  String server = read_config_string(CONFIG_FNAME_SYSLOG);
  int delim = server.indexOf(':');
  if( delim >= 0 ){
    String host = server.substring(0, delim);
    String port = server.substring(delim + 1);
    syslog_changeServer(host.c_str(), port.toInt());
  }

  return 0;
}

void loopModule_esp32(void)
{
#if defined(ARDUINO_ESP32C3_DEV)
#elif defined(ARDUINO_ESP32C3U_DEV)
#elif defined(ARDUINO_ESP32S3_DEV)
#else
  M5.update();
#endif
}

JsModuleEntry esp32_module = {
  initialize_esp32,
  addModule_esp32,
  loopModule_esp32,
  NULL
};

JsModuleEntry console_module = {
  NULL,
  addModule_console,
  NULL,
  NULL
};
