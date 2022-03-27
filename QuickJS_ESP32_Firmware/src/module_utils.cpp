#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "quickjs.h"
#include "main_config.h"
#include "module_type.h"
#include "module_utils.h"
#include <base64.hpp>

unsigned long b64_encode_length(unsigned long input_length)
{
  return encode_base64_length(input_length);
}

unsigned long b64_encode(const unsigned char input[], unsigned long input_length, char output[])
{
  return encode_base64((unsigned char*)input, input_length, (unsigned char*)output);
}

unsigned long b64_decode_length(const char input[])
{
  return decode_base64_length((unsigned char*)input);
}

unsigned long b64_decode(const char input[], unsigned char output[])
{
  return decode_base64((unsigned char*)input, output);
}

String urlencode(String str)
{
  String encodedString = "";
  char c;
  char code0;
  char code1;
//    char code2;
  for (int i = 0; i < str.length(); i++){
    c = str.charAt(i);
    if (c == ' '){
      encodedString += '+';
    } else if (isalnum(c)){
      encodedString += c;
    }else{
      code1 = (c & 0xf) +'0';
      if ((c & 0xf) > 9){
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c>>4) & 0xf;
      code0 = c + '0';
      if (c > 9){
        code0 = c - 10 + 'A';
      }
//        code2 = '\0';
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
//encodedString += code2;
    }
//      yield();
  }
  return encodedString;
}

static JSValue utils_http_json(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv, int magic)
{
  HTTPClient http;
  const char *url = JS_ToCString(ctx, argv[0]);
  if (magic == 0){
    // HTTP POST JSON
    Serial.println(url);
    http.begin(url); //HTTP
    http.addHeader("Content-Type", "application/json");
  }else if( magic == 1){
    // HTTP GET
    if( argc >= 2 && argv[1] != JS_UNDEFINED ){
      JSPropertyEnum *atoms;
      uint32_t len;
      int ret = JS_GetOwnPropertyNames(ctx, &atoms, &len, argv[1], JS_GPN_ENUM_ONLY | JS_GPN_STRING_MASK);
      if (ret != 0){
        JS_FreeCString(ctx, url);
        return JS_EXCEPTION;
      }

      String url_str = String(url);
      bool first = true;
      for (int i = 0; i < len; i++){
        JSAtom atom = atoms[i].atom;
        const char *name = JS_AtomToCString(ctx, atom);
        if( name != NULL ){
          JSValue value = JS_GetPropertyStr(ctx, argv[1], name);
          const char *str = JS_ToCString(ctx, value);
          if( str != NULL ){
            Serial.printf("%s=%s\n", name, str);

            if( first ){
              if (url_str.indexOf('?') >= 0)
                url_str += "&";
              else
                url_str += "?";
              first = false;
            }else{
              url_str += "&";
            }
            url_str += name;
            url_str += "=";
            url_str += urlencode(str);
            JS_FreeCString(ctx, str);
          }
          JS_FreeCString(ctx, name);
        }
        JS_FreeAtom(ctx, atom);
      }

      Serial.println(url_str);
      http.begin(url_str); //HTTP
    }else{
      Serial.println(url);
      http.begin(url); //HTTP
    }
  }else{
    JS_FreeCString(ctx, url);
    return JS_EXCEPTION;
  }
  JS_FreeCString(ctx, url);

  if( argc >= 3 && argv[2] != JS_UNDEFINED ){
    JSPropertyEnum *atoms;
    uint32_t len;
    int ret = JS_GetOwnPropertyNames(ctx, &atoms, &len, argv[2], JS_GPN_ENUM_ONLY | JS_GPN_STRING_MASK);
    if (ret != 0){
      http.end();
      return JS_EXCEPTION;
    }

    for (int i = 0; i < len; i++){
      JSAtom atom = atoms[i].atom;
      const char *name = JS_AtomToCString(ctx, atom);
      if( name != NULL ){
        JSValue value = JS_GetPropertyStr(ctx, argv[2], name);
        const char *str = JS_ToCString(ctx, value);
        if( str != NULL ){
          Serial.printf("%s=%s\n", name, str);

          http.addHeader(name, str);
          JS_FreeCString(ctx, str);
        }
        JS_FreeCString(ctx, name);
      }
      JS_FreeAtom(ctx, atom);
    }
  }

  JSValue value = JS_EXCEPTION;
  int status_code;
  if (magic == 0){
    // HTTP POST JSON
    if( argc >= 2 && argv[1] != JS_UNDEFINED ){
      JSValue json = JS_JSONStringify(ctx, argv[1], JS_UNDEFINED, JS_UNDEFINED);
      if( json == JS_UNDEFINED ){
        http.end();
        return JS_EXCEPTION;
      }
      const char *body = JS_ToCString(ctx, json);
      if( body == NULL ){
        http.end();
        JS_FreeValue(ctx, json);
        return JS_EXCEPTION;
      }
      Serial.printf("body=%s\n", body);

      status_code = http.POST(body);
      JS_FreeCString(ctx, body);
      JS_FreeValue(ctx, json);
      }else{
        status_code = http.POST("{}");
      }
    }else{
      // HTTP GET
      status_code = http.GET();
    }

  if (status_code == 200){
    String result = http.getString();
    const char *buffer = result.c_str();
    value = JS_ParseJSON(ctx, buffer, strlen(buffer), "json");
  }else{
    Serial.printf("status_code=%d\n", status_code);
    goto end;
  }

end:
  http.end();
  return value;
}

static JSValue utils_http_get_text(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  HTTPClient http;
  const char *url = JS_ToCString(ctx, argv[0]);
  if( argc >= 2 && argv[1] != JS_UNDEFINED ){
    JSPropertyEnum *atoms;
    uint32_t len;
    int ret = JS_GetOwnPropertyNames(ctx, &atoms, &len, argv[1], JS_GPN_ENUM_ONLY | JS_GPN_STRING_MASK);
    if (ret != 0){
      JS_FreeCString(ctx, url);
      return JS_EXCEPTION;
    }

    String url_str = String(url);
    bool first = true;
    for (int i = 0; i < len; i++){
      JSAtom atom = atoms[i].atom;
      const char *name = JS_AtomToCString(ctx, atom);
      if( name != NULL ){
        JSValue value = JS_GetPropertyStr(ctx, argv[1], name);
        const char *str = JS_ToCString(ctx, value);
        if( str != NULL ){
          Serial.printf("%s=%s\n", name, str);

          if( first ){
            if (url_str.indexOf('?') >= 0)
              url_str += "&";
            else
              url_str += "?";
            first = false;
          }else{
            url_str += "&";
          }
          url_str += name;
          url_str += "=";
          url_str += urlencode(str);
          JS_FreeCString(ctx, str);
        }
        JS_FreeCString(ctx, name);
      }
      JS_FreeAtom(ctx, atom);
    }

    Serial.println(url_str);
    http.begin(url_str); //HTTP
    }else{
    Serial.println(url);
    http.begin(url); //HTTP
  }
  JS_FreeCString(ctx, url);

  if( argc >= 3 && argv[2] != JS_UNDEFINED ){
    JSPropertyEnum *atoms;
    uint32_t len;
    int ret = JS_GetOwnPropertyNames(ctx, &atoms, &len, argv[2], JS_GPN_ENUM_ONLY | JS_GPN_STRING_MASK);
    if (ret != 0){
      http.end();
      return JS_EXCEPTION;
    }

    for (int i = 0; i < len; i++){
      JSAtom atom = atoms[i].atom;
      const char *name = JS_AtomToCString(ctx, atom);
      if( name != NULL ){
        JSValue value = JS_GetPropertyStr(ctx, argv[2], name);
        const char *str = JS_ToCString(ctx, value);
        if( str != NULL ){
          Serial.printf("%s=%s\n", name, str);

          http.addHeader(name, str);
          JS_FreeCString(ctx, str);
        }
        JS_FreeCString(ctx, name);
      }
      JS_FreeAtom(ctx, atom);
    }
  }

  JSValue value = JS_EXCEPTION;
  int status_code = http.GET();
  if (status_code == 200){
    String result = http.getString();
    value = JS_NewString(ctx, result.c_str());
  }else{
    Serial.printf("status_code=%d\n", status_code);
    goto end;
  }

end:
  http.end();
  return value;
}

static JSValue utils_urlencode(JSContext * ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *str = JS_ToCString(ctx, argv[0]);
  String encoded = urlencode(str);
  JSValue value = JS_NewString(ctx, encoded.c_str());
  JS_FreeCString(ctx, str);
  return value;
}

static JSValue utils_base64(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv, int magic)
{
  if( magic == 0 ){
    uint32_t unit_num;
    uint8_t unit_size;
    uint8_t *p_buffer;
    JSValue vbuffer = getArrayBuffer(ctx, argv[0], (void**)p_buffer, &unit_size, &unit_num);
    if( JS_IsNull(vbuffer) )
      return JS_EXCEPTION;
    if( unit_size != 1 ){
      JS_FreeValue(ctx, vbuffer);
        return JS_EXCEPTION;
    }
    unsigned int len = b64_encode_length(unit_num);
    char *str = (char *)malloc(len + 1);
    if (str == NULL){
      JS_FreeValue(ctx, vbuffer);
      return JS_EXCEPTION;
    }
    b64_encode(p_buffer, unit_num, str);
    JS_FreeValue(ctx, vbuffer);
    JSValue value = JS_NewString(ctx, str);
    free(str);

    return value;
  }else if( magic == 1 ){
    const char *b64 = JS_ToCString(ctx, argv[0]);
    if( b64 == NULL )
      return JS_EXCEPTION;
    unsigned int length = b64_decode_length(b64);
    unsigned char *buffer = (unsigned char *)malloc(length);
    if( buffer == NULL ){
      JS_FreeCString(ctx, b64);
      return JS_EXCEPTION;
    }
    b64_decode(b64, buffer);
    JS_FreeCString(ctx, b64);
    JSValue value = JS_NewArrayBufferCopy(ctx, buffer, length);
    free(buffer);

    return value;
  }

  return JS_EXCEPTION;
}

static const JSCFunctionListEntry utils_funcs[] = {
    JSCFunctionListEntry{"httpPostJson", 0, JS_DEF_CFUNC, 0, {
                           func : {3, JS_CFUNC_generic_magic, {generic_magic : utils_http_json}}
                         }},
    JSCFunctionListEntry{"httpGetJson", 0, JS_DEF_CFUNC, 1, {
                           func : {3, JS_CFUNC_generic_magic, {generic_magic : utils_http_json}}
                         }},
    JSCFunctionListEntry{"httpGetText", 0, JS_DEF_CFUNC, 0, {
                           func : {3, JS_CFUNC_generic, utils_http_get_text}
                         }},
    JSCFunctionListEntry{"urlencode", 0, JS_DEF_CFUNC, 0, {
                           func : {1, JS_CFUNC_generic, utils_urlencode}
                         }},
    JSCFunctionListEntry{"base64Encode", 0, JS_DEF_CFUNC, 0, {
                           func : {1, JS_CFUNC_generic_magic, {generic_magic : utils_base64}}
                         }},
    JSCFunctionListEntry{"base64Decode", 0, JS_DEF_CFUNC, 1, {
                           func : {1, JS_CFUNC_generic_magic, {generic_magic : utils_base64}}
                         }},
};

JSModuleDef *addModule_utils(JSContext * ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "Utils", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, utils_funcs,
                            sizeof(utils_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, utils_funcs,
        sizeof(utils_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

JsModuleEntry utils_module = {
  NULL,
  addModule_utils,
  NULL,
  NULL
};

long http_get(String url, String *response)
{
  Serial.println(url);

  HTTPClient http;
  Serial.print("[HTTP] GET begin...\n");
  // configure traged server and url
  http.begin(url); //HTTP

  // start connection and send HTTP header
  int httpCode = http.GET();

  // HTTP header has been send and Server response header has been handled
  Serial.printf("[HTTP] GET... code: %d\n", httpCode);

  // file found at server
  if (httpCode == HTTP_CODE_OK){
    int len = http.getSize();
    Serial.printf("[HTTP] Content-Length=%d\n", len);
    
    *response = http.getString();

    http.end();
    delay(100);
    return 0;
  }else{
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    http.end();
    return -1;
  }
}

long http_get_binary(String url, uint8_t * p_buffer, unsigned long *p_len)
{
  Serial.println(url);

  HTTPClient http;
  Serial.print("[HTTP] GET begin...\n");
  // configure traged server and url
  http.begin(url); //HTTP

  // start connection and send HTTP header
  int httpCode = http.GET();
  unsigned long index = 0;

  // HTTP header has been send and Server response header has been handled
  Serial.printf("[HTTP] GET... code: %d\n", httpCode);

  // file found at server
  if (httpCode == HTTP_CODE_OK){
    // get tcp stream
    WiFiClient *stream = http.getStreamPtr();

    // get lenght of document (is -1 when Server sends no Content-Length header)
    int len = http.getSize();
    Serial.printf("[HTTP] Content-Length=%d\n", len);
    if (len != -1 && len > *p_len){
      Serial.printf("[HTTP] buffer size over\n");
      http.end();
      return -1;
    }

    // read all data from server
    while (http.connected() && (len > 0 || len == -1)){
      // get available data size
      size_t size = stream->available();

      if (size > 0){
        if ((index + size) > *p_len){
          Serial.printf("[HTTP] buffer size over\n");
          http.end();
          return -1;
        }
        int c = stream->readBytes(&p_buffer[index], size);

        index += c;
        if (len > 0){
          len -= c;
        }
      }
      delay(1);
    }
  }else{
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    http.end();
    return -1;
  }

  http.end();
  *p_len = index;

  delay(100);

  return 0;
}

long http_get_json(String url, JsonDocument * doc)
{
  Serial.println(url);

  HTTPClient http;
  Serial.print("[HTTP] GET begin...\n");
  // configure traged server and url
  http.begin(url); //HTTP

  // start connection and send HTTP header
  int httpCode = http.GET();

  // HTTP header has been send and Server response header has been handled
  Serial.printf("[HTTP] GET... code: %d\n", httpCode);

  // file found at server
  if (httpCode == HTTP_CODE_OK){
    // get tcp stream
    int len = http.getSize();
    Serial.printf("[HTTP] Content-Length=%d\n", len);
    Stream *stream = http.getStreamPtr();
    DeserializationError err = deserializeJson(*doc, *stream);
    if (err){
      Serial.print("Deserialize error: ");
      Serial.println(err.c_str());
      return -1;
    }
  }else{
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    http.end();
    return -1;
  }

  http.end();
  delay(100);

  return 0;
}

JSValue getArrayBuffer(JSContext *ctx, JSValue value, void** p_buffer, uint8_t *p_unit_size, uint32_t *p_unit_num)
{
  JSValue vlen = JS_GetPropertyStr(ctx, value, "length");
  if( JS_IsException(vlen) )
    return JS_NULL;
  JS_ToUint32(ctx, p_unit_num, vlen);
  JS_FreeValue(ctx, vlen);

  JSValue vbuffer = JS_GetPropertyStr(ctx, value, "buffer");
  if( JS_IsException(vbuffer) ){
    JS_FreeValue(ctx, vbuffer);
    return JS_NULL;
  }
  size_t bsize;
  *p_buffer = (void*)JS_GetArrayBuffer(ctx, &bsize, vbuffer);
  if( *p_buffer == NULL ){
    JS_FreeValue(ctx, vbuffer);
    return JS_NULL;
  }

  *p_unit_size = bsize / *p_unit_num;

  return vbuffer;
}

bool is_wifi_connected(void)
{
  return WiFi.status() == WL_CONNECTED;
}
