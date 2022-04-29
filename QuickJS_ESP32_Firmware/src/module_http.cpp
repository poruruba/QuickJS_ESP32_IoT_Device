#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "quickjs.h"
#include "main_config.h"
#include "module_type.h"
#include "module_utils.h"

#define HTTP_TYPE_MASK  0xF

#define HTTP_RESP_SHIFT   0
#define HTTP_RESP_NONE    0x0
#define HTTP_RESP_TEXT    0x1
#define HTTP_RESP_JSON    0x2
#define HTTP_RESP_BINARY  0x3

#define HTTP_METHOD_SHIFT   4
#define HTTP_METHOD_GET    0x0
#define HTTP_METHOD_POST   0x1

#define HTTP_CONTENT_SHIFT   8
#define HTTP_CONTENT_NONE    0x0
#define HTTP_CONTENT_JSON    0x1
#define HTTP_CONTENT_URLENCODE    0x2
#define HTTP_CONTENT_FORMDATA    0x3

#define HTTP_TYPE_POST_JSON ((HTTP_METHOD_POST << HTTP_METHOD_SHIFT) | (HTTP_CONTENT_JSON << HTTP_CONTENT_SHIFT) | (HTTP_RESP_JSON << HTTP_RESP_SHIFT))
#define HTTP_TYPE_GET_JSON  ((HTTP_METHOD_GET << HTTP_METHOD_SHIFT) | (HTTP_CONTENT_JSON << HTTP_CONTENT_SHIFT) | (HTTP_RESP_JSON << HTTP_RESP_SHIFT))
#define HTTP_TYPE_POST_TEXT ((HTTP_METHOD_POST << HTTP_METHOD_SHIFT) | (HTTP_CONTENT_JSON << HTTP_CONTENT_SHIFT) | (HTTP_RESP_TEXT << HTTP_RESP_SHIFT))
#define HTTP_TYPE_GET_TEXT  ((HTTP_METHOD_GET << HTTP_METHOD_SHIFT) | (HTTP_CONTENT_JSON << HTTP_CONTENT_SHIFT) | (HTTP_RESP_TEXT << HTTP_RESP_SHIFT))

static JSValue http_gateway(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv, int magic)
{
  JSValue obj = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, obj, "url", JS_DupValue(ctx, argv[0]));
  if (argc >= 1)
    JS_SetPropertyStr(ctx, obj, "params", JS_DupValue(ctx, argv[1]));
  if (argc >= 2)
    JS_SetPropertyStr(ctx, obj, "headers", JS_DupValue(ctx, argv[2]));
  JS_SetPropertyStr(ctx, obj, "method_type", magic);

  JSValue json = JS_JSONStringify(ctx, obj, JS_UNDEFINED, JS_UNDEFINED);
  JS_FreeValue(ctx, obj);
  if( json == JS_UNDEFINED ){
    return JS_EXCEPTION;
  }
  const char *body = JS_ToCString(ctx, json);
  if (body == NULL){
    JS_FreeValue(ctx, json);
    return JS_EXCEPTION;
  }

  String server = read_config_string(CONFIG_FNAME_GATEWAY);
  HTTPClient http;
  http.begin(server); //HTTP
  http.addHeader("Content-Type", "application/json");

  int status_code;
  // HTTP POST JSON
  status_code = http.POST(body);
  JS_FreeCString(ctx, body);
  JS_FreeValue(ctx, json);

  uint8_t response_type = ( magic >> HTTP_RESP_SHIFT ) & HTTP_TYPE_MASK;
  JSValue value = JS_EXCEPTION;
  if (status_code != 200){
    Serial.printf("status_code=%d\n", status_code);
    goto end;
  }

  if (response_type == HTTP_RESP_JSON ){
    String result = http.getString();
    const char *buffer = result.c_str();
    value = JS_ParseJSON(ctx, buffer, strlen(buffer), "json");
  }else if( response_type == HTTP_RESP_TEXT ){
    String result = http.getString();
    const char *buffer = result.c_str();
    value = JS_NewString(ctx, buffer);
  }else if( response_type == HTTP_RESP_BINARY ){
    String result = http.getString();
    const char *b64 = result.c_str();
    unsigned long binlen = b64_decode_length(b64);
    unsigned char *bin = (unsigned char*)malloc(binlen);
    if( bin == NULL )
      goto end;
    b64_decode(b64, bin);

    value = JS_NewArrayBufferCopy(ctx, bin, binlen);
    free(bin);
  }else if( response_type == HTTP_RESP_NONE ){
    value = JS_UNDEFINED;
  }

end:
  http.end();

  return value;
}

static JSValue http_fetch(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  if( argc <= 1 )
    return JS_EXCEPTION;

  int32_t type;
  JS_ToInt32(ctx, &type, argv[0]);

  return http_gateway(ctx, jsThis, argc - 1, &argv[1], type);
}

static JSValue http_setGatewayServer(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *url = JS_ToCString(ctx, argv[0]);
  if( url == NULL )
    return JS_EXCEPTION;

  long ret;  
  ret = write_config_string(CONFIG_FNAME_GATEWAY, url);
  JS_FreeCString(ctx, url);
  if( ret != 0 )
    return JS_EXCEPTION;

  return JS_UNDEFINED;
}

static JSValue http_getGatewayServer(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  String url = read_config_string(CONFIG_FNAME_GATEWAY);

  return JS_NewString(ctx, url.c_str());
}

static const JSCFunctionListEntry http_funcs[] = {
    JSCFunctionListEntry{"postJson", 0, JS_DEF_CFUNC, HTTP_TYPE_POST_JSON, {
                           func : {3, JS_CFUNC_generic_magic, {generic_magic : http_gateway}}
                         }},
    JSCFunctionListEntry{"getJson", 0, JS_DEF_CFUNC, HTTP_TYPE_GET_JSON, {
                           func : {3, JS_CFUNC_generic_magic, {generic_magic : http_gateway}}
                         }},
    JSCFunctionListEntry{"postText", 0, JS_DEF_CFUNC, HTTP_TYPE_POST_TEXT, {
                           func : {3, JS_CFUNC_generic_magic, {generic_magic : http_gateway}}
                         }},
    JSCFunctionListEntry{"getText", 0, JS_DEF_CFUNC, HTTP_TYPE_GET_TEXT, {
                           func : {3, JS_CFUNC_generic_magic, {generic_magic : http_gateway}}
                         }},
    JSCFunctionListEntry{"fetch", 0, JS_DEF_CFUNC, 0, {
                           func : {4, JS_CFUNC_generic, http_fetch}
                         }},
    JSCFunctionListEntry{"setGatewayServer", 0, JS_DEF_CFUNC, 0, {
                           func : {1, JS_CFUNC_generic, http_setGatewayServer}
                         }},
    JSCFunctionListEntry{"getGatewayServer", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, http_getGatewayServer}
                         }},
    JSCFunctionListEntry{
        "resp_none", 0, JS_DEF_PROP_INT32, 0, {
          i32 : (HTTP_RESP_NONE << HTTP_RESP_SHIFT)
        }},
    JSCFunctionListEntry{
        "resp_text", 0, JS_DEF_PROP_INT32, 0, {
          i32 : (HTTP_RESP_TEXT << HTTP_RESP_SHIFT)
        }},
    JSCFunctionListEntry{
        "resp_json", 0, JS_DEF_PROP_INT32, 0, {
          i32 : (HTTP_RESP_JSON << HTTP_RESP_SHIFT)
        }},
    JSCFunctionListEntry{
        "resp_binary", 0, JS_DEF_PROP_INT32, 0, {
          i32 : (HTTP_RESP_BINARY << HTTP_RESP_SHIFT)
        }},
    JSCFunctionListEntry{
        "method_get", 0, JS_DEF_PROP_INT32, 0, {
          i32 : (HTTP_METHOD_GET << HTTP_METHOD_SHIFT)
        }},
    JSCFunctionListEntry{
        "method_post", 0, JS_DEF_PROP_INT32, 0, {
          i32 : (HTTP_METHOD_POST << HTTP_METHOD_SHIFT)
        }},
    JSCFunctionListEntry{
        "content_none", 0, JS_DEF_PROP_INT32, 0, {
          i32 : (HTTP_CONTENT_NONE << HTTP_CONTENT_SHIFT)
        }},
    JSCFunctionListEntry{
        "content_json", 0, JS_DEF_PROP_INT32, 0, {
          i32 : (HTTP_CONTENT_JSON << HTTP_CONTENT_SHIFT)
        }},
    JSCFunctionListEntry{
        "content_urlencode", 0, JS_DEF_PROP_INT32, 0, {
          i32 : (HTTP_CONTENT_URLENCODE << HTTP_CONTENT_SHIFT)
        }},
    JSCFunctionListEntry{
        "content_formdata", 0, JS_DEF_PROP_INT32, 0, {
          i32 : (HTTP_CONTENT_FORMDATA << HTTP_CONTENT_SHIFT)
        }},
};

JSModuleDef *addModule_http(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "Http", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, http_funcs,
                            sizeof(http_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, http_funcs,
        sizeof(http_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

JsModuleEntry http_module = {
  NULL,
  addModule_http,
  NULL,
  NULL
};