#include <Arduino.h>
#include "main_config.h"

#ifdef _MQTT_ENABLE_

#include <WiFi.h>
#include "quickjs.h"
#include "quickjs_esp32.h"

#include "module_mqtt.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define DEFAULT_MQTT_BUFFER_SIZE 256
#define MQTT_CONNECT_TRY_COUNT 5


static JSContext *g_ctx = NULL;

static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);
static char *g_client_name = NULL;
static char *g_topic_name = NULL;
static JSValue g_callback_func = JS_UNDEFINED;
static char *g_received_topic = NULL;
static byte *g_received_payload = NULL;
static uint16_t g_buffer_size;
static char *g_broker_host = NULL;

static void mqttCallback(char* topic, byte* payload, unsigned int length)
{
  if(g_ctx == NULL || g_callback_func == JS_UNDEFINED)
    return;
  
  if( length <= g_buffer_size - 1 ){
    if( g_received_topic != NULL )
      free(g_received_topic);
    g_received_topic = (char*)malloc(strlen(topic) + 1);
    strcpy(g_received_topic, topic);
    g_received_payload = payload;
    g_received_payload[length] = '\0';
  }
}

static long mqttUnsubscribe(void){
  if( g_callback_func != JS_UNDEFINED ){
    mqttClient.unsubscribe(g_topic_name);
    JS_FreeValue(g_ctx, g_callback_func);
    g_callback_func = JS_UNDEFINED;
  }
  g_received_payload = NULL;

  return 0;
}

static long mqttSubscribe(const char* topic_name, JSValue callback){
  mqttUnsubscribe();
  if( mqttClient.subscribe(topic_name) ){
    if( g_topic_name != NULL )
      free(g_topic_name);
    g_topic_name = (char*)malloc(strlen(topic_name) + 1);
    strcpy(g_topic_name, topic_name);
    g_callback_func = callback;
    return 0;
  }else{
    return -1;
  }
}

static void mqttDisconnect(void){
  mqttUnsubscribe();
  if (mqttClient.connected())
      mqttClient.disconnect();

  g_ctx = NULL;
}

static boolean mqttConnect(JSContext *ctx, const char *client_name, const char *broker_url, uint16_t broker_port, uint16_t buffer_size, const char *username, const char *password)
{
  mqttDisconnect();

  if( g_client_name != NULL )
    free(g_client_name);
  g_client_name = (char*)malloc(strlen(client_name) + 1);
  strcpy(g_client_name, client_name);
  if( g_broker_host != NULL )
    free(g_broker_host);
  g_broker_host = (char*)malloc(strlen(broker_url) + 1);
  strcpy(g_broker_host, broker_url);

  g_buffer_size = buffer_size;
  mqttClient.setBufferSize(g_buffer_size);
  mqttClient.setCallback(mqttCallback);
  mqttClient.setServer(g_broker_host, broker_port);

  g_ctx = ctx;

  return mqttClient.connect(g_client_name, username, password);
}

static JSValue mqtt_connect(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *client_name = JS_ToCString(ctx, argv[0]);
  if( client_name == NULL )
    return JS_EXCEPTION;

  uint32_t buffer_size = DEFAULT_MQTT_BUFFER_SIZE;
  if( argc >= 2 )
      JS_ToUint32(ctx, &buffer_size, argv[1]);

  const char *username = NULL;
  if( argc >= 3 )
    username = JS_ToCString(ctx, argv[2]);
  const char *password = NULL;
  if( argc >= 4 )
    password = JS_ToCString(ctx, argv[3]);

  String server = read_config_string(CONFIG_FNAME_MQTT);
  int delim = server.indexOf(':');
  if( delim < 0 ){
    JS_FreeCString(ctx, client_name);
    return JS_EXCEPTION;
  }
  String host = server.substring(0, delim);
  String port = server.substring(delim + 1);
  mqttConnect(ctx, client_name, host.c_str(), port.toInt(), buffer_size, username, password);
  JS_FreeCString(ctx, client_name);
  if( username != NULL )
    JS_FreeCString(ctx, username);
  if( password != NULL )
    JS_FreeCString(ctx, password);

  return JS_UNDEFINED;
}

static JSValue mqtt_get_payload(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  if (g_received_payload == NULL)
    return JS_EXCEPTION;
  JSValue obj = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, obj, "payload", JS_NewString(ctx, (const char *)g_received_payload));
  JS_SetPropertyStr(ctx, obj, "topic", JS_NewString(ctx, (const char *)g_received_topic));

  return obj;
}

static JSValue mqtt_disconnect(JSContext * ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  mqttDisconnect();
  return JS_UNDEFINED;
}

static JSValue mqtt_subscribe(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *topic = JS_ToCString(ctx, argv[0]);
  if( topic == NULL )
    return JS_EXCEPTION;

  JSValue func = JS_DupValue(g_ctx, argv[1]);
  long ret = mqttSubscribe(topic, func);
  JS_FreeCString(ctx, topic);
  if( ret != 0 )
    return JS_EXCEPTION;

  return JS_UNDEFINED;
}

static JSValue mqtt_unsubscribe(JSContext * ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  mqttUnsubscribe();

  return JS_UNDEFINED;
}

static JSValue mqtt_publish(JSContext * ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *topic = JS_ToCString(ctx, argv[0]);
  if( topic == NULL )
    return JS_EXCEPTION;

  const char *payload = JS_ToCString(ctx, argv[1]);
  if( payload == NULL ){
    JS_FreeCString(ctx, topic);
    return JS_EXCEPTION;
  }

  bool ret = mqttClient.publish(topic, payload);
  JS_FreeCString(ctx, payload);
  JS_FreeCString(ctx, topic);

  if( ret )
    return JS_UNDEFINED;
  else
    return JS_EXCEPTION;
}


static JSValue mqtt_setServer(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *host = JS_ToCString(ctx, argv[0]);
  if( host == NULL )
    return JS_EXCEPTION;
  int32_t port;
  JS_ToInt32(ctx, &port, argv[1]);

  String server(host);
  server += ":";
  server += String(port);

  long ret;  
  ret = write_config_string(CONFIG_FNAME_MQTT, server.c_str());
  if( ret != 0 ){
    JS_FreeCString(ctx, host);
    return JS_EXCEPTION;
  }

  return JS_UNDEFINED;
}

static JSValue mqtt_getServer(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  String server = read_config_string(CONFIG_FNAME_MQTT);
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

static const JSCFunctionListEntry mqtt_funcs[] = {
    JSCFunctionListEntry{
        "connect", 0, JS_DEF_CFUNC, 0, {
          func : {4, JS_CFUNC_generic, mqtt_connect}
        }},
    JSCFunctionListEntry{
        "disconnect", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, mqtt_disconnect}
        }},
    JSCFunctionListEntry{
        "subscribe", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, mqtt_subscribe}
        }},
    JSCFunctionListEntry{
        "unsubscribe", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, mqtt_unsubscribe}
        }},
    JSCFunctionListEntry{
        "publish", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, mqtt_publish}
        }},
    JSCFunctionListEntry{
        "getPayload", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, mqtt_get_payload}
        }},
    JSCFunctionListEntry{
        "setServer", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, mqtt_setServer}
        }},
    JSCFunctionListEntry{
        "getServer", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, mqtt_getServer}
        }},
};

JSModuleDef *addModule_mqtt(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;

  mod = JS_NewCModule(ctx, "Mqtt", [](JSContext *ctx, JSModuleDef *m) {
          return JS_SetModuleExportList(
              ctx, m, mqtt_funcs,
              sizeof(mqtt_funcs) / sizeof(JSCFunctionListEntry));
        });
  if (mod) {
    JS_AddModuleExportList(
        ctx, mod, mqtt_funcs,
        sizeof(mqtt_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

void loopModule_mqtt(void){
  if( g_ctx != NULL ){
    mqttClient.loop();
    for( int i = 0 ; !mqttClient.connected() && i < MQTT_CONNECT_TRY_COUNT ; i++ ){
      Serial.println("Mqtt Reconnecting");
      if (mqttClient.connect(g_client_name)){
        if( g_callback_func != JS_UNDEFINED )
          mqttClient.subscribe(g_topic_name);
        Serial.println("Mqtt Reconnected");

        break;
      }

      delay(500);
    }

    if( g_received_payload != NULL && g_callback_func != JS_UNDEFINED){
      ESP32QuickJS *qjs = (ESP32QuickJS *)JS_GetContextOpaque(g_ctx);
      JSValue ret = qjs->callJsFunc(g_ctx, g_callback_func, g_callback_func);
      JS_FreeValue(g_ctx, ret);
      g_received_payload = NULL;
    }
  }
}

void endModule_mqtt(void){
  mqttDisconnect();
}

JsModuleEntry mqtt_module = {
  NULL,
  addModule_mqtt,
  loopModule_mqtt,
  endModule_mqtt
};

#endif
