#include <Arduino.h>
#include "quickjs.h"
#include "module_type.h"
#include "module_utils.h"
#include <WiFi.h>
#include <WiFiUdp.h>

static WiFiUDP udp;

static JSValue esp32_udp_recvBegin(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  uint32_t port;
  JS_ToUint32(ctx, &port, argv[0]);

  udp.stop();
  udp.begin(port);

  return JS_UNDEFINED;
}

static JSValue esp32_udp_sendBinary(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  uint32_t host, port;
  JS_ToUint32(ctx, &host, argv[0]);
  JS_ToUint32(ctx, &port, argv[1]);

  uint8_t *p_buffer;
  uint8_t unit_size;
  uint32_t bsize;
  JSValue vbuffer = getArrayBuffer(ctx, argv[2], (void**)&p_buffer, &unit_size, &bsize);
  if( JS_IsNull(vbuffer) )
    return JS_EXCEPTION;
  if( unit_size != 1 ){
    JS_FreeValue(ctx, vbuffer);
    return JS_EXCEPTION;
  }

  char temp[16];
  sprintf(temp, "%d.%d.%d.%d", (host >> 24) & 0xff, (host >> 16) & 0xff, (host >> 8) & 0xff, host & 0xff);
  udp.beginPacket(temp, port);
  udp.write(p_buffer, bsize);
  udp.endPacket();

  JS_FreeValue(ctx, vbuffer);

  return JS_UNDEFINED;
}

static JSValue esp32_udp_sendText(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  uint32_t host, port;
  JS_ToUint32(ctx, &host, argv[0]);
  JS_ToUint32(ctx, &port, argv[1]);

  const char *text = JS_ToCString(ctx, argv[2]);
  if( text == NULL )
    return JS_EXCEPTION;

  char temp[16];
  sprintf(temp, "%d.%d.%d.%d", (host >> 24) & 0xff, (host >> 16) & 0xff, (host >> 8) & 0xff, host & 0xff);
  udp.beginPacket(temp, port);
  udp.write((const uint8_t*)text, strlen(text));
  udp.endPacket();

  JS_FreeCString(ctx, text);

  return JS_UNDEFINED;
}

static JSValue esp32_udp_recvStop(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  udp.stop();

  return JS_UNDEFINED;
}

static JSValue esp32_udp_checkRecvBinary(JSContext *ctx, JSValueConst jsThis, int argc,
                               JSValueConst *argv)
{
  int packetSize = udp.parsePacket();
  if( packetSize <= 0 )
    return JS_NULL;

  uint8_t *p_buffer = (uint8_t*)malloc(packetSize);
  if( p_buffer == NULL )
    return JS_EXCEPTION;
  
  int len = udp.read(p_buffer, packetSize);
  if( len <= 0 ){
    free(p_buffer);
    return JS_EXCEPTION;
  }

  JSValue value = JS_NewArrayBufferCopy(ctx, p_buffer, len);
  free(p_buffer);

  return value;
}

static JSValue esp32_udp_checkRecvText(JSContext *ctx, JSValueConst jsThis, int argc,
                                      JSValueConst *argv)
{
  int packetSize = udp.parsePacket();
  if( packetSize <= 0 )
    return JS_NULL;

  char *p_buffer = (char*)malloc(packetSize + 1);
  if( p_buffer == NULL )
    return JS_EXCEPTION;
  
  int len = udp.read(p_buffer, packetSize);
  if( len <= 0 ){
    free(p_buffer);
    return JS_EXCEPTION;
  }
  p_buffer[len] = '\0';

  JSValue value = JS_NewString(ctx, p_buffer);
  free(p_buffer);

  return value;
}


static const JSCFunctionListEntry udp_funcs[] = {
    JSCFunctionListEntry{"recvBegin", 0, JS_DEF_CFUNC, 0, {
                           func : {1, JS_CFUNC_generic, esp32_udp_recvBegin}
                         }},
    JSCFunctionListEntry{"sendText", 0, JS_DEF_CFUNC, 0, {
                           func : {3, JS_CFUNC_generic, esp32_udp_sendText}
                         }},
    JSCFunctionListEntry{"sendBinary", 0, JS_DEF_CFUNC, 0, {
                           func : {3, JS_CFUNC_generic, esp32_udp_sendBinary}
                         }},
    JSCFunctionListEntry{"recvStop", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, esp32_udp_recvStop}
                         }},
    JSCFunctionListEntry{"checkRecvText", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, esp32_udp_checkRecvText}
                         }},
    JSCFunctionListEntry{"checkRecvBinary", 0, JS_DEF_CFUNC, 0, {
                           func : {0, JS_CFUNC_generic, esp32_udp_checkRecvBinary}
                         }},
};

JSModuleDef *addModule_udp(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;

  mod = JS_NewCModule(ctx, "Udp", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, udp_funcs,
                            sizeof(udp_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, udp_funcs,
        sizeof(udp_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

void endModule_udp(void){
  udp.stop();
}

JsModuleEntry udp_module = {
  NULL,
  addModule_udp,
  NULL,
  endModule_udp
};
