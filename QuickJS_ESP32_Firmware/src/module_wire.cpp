#include <Arduino.h>
#include <Wire.h>
#include "quickjs.h"
#include "module_wire.h"
#include "module_type.h"

static JSValue esp32_wire_begin(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv, int magic)
{
  TwoWire *wire;
  if (magic == 0)
    wire = &Wire;
  else if (magic == 1)
    wire = &Wire1;
  else
    return JS_EXCEPTION;

  int32_t sda = -1;
  int32_t scl = -1;
  uint32_t frequency = 0;
  if( argc > 0 )
    JS_ToInt32(ctx, &sda, argv[0]);
  if( argc > 1 )
    JS_ToInt32(ctx, &scl, argv[1]);
  if( argc > 2)
    JS_ToUint32(ctx, &frequency, argv[2]);
  bool ret = wire->begin(sda, scl, frequency);

  return JS_NewBool(ctx, ret);
}

static JSValue esp32_wire_requestFrom(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv, int magic)
{
  TwoWire *wire;
  if (magic == 0)
    wire = &Wire;
  else if (magic == 1)
    wire = &Wire1;
  else
    return JS_EXCEPTION;

  uint32_t address;
  uint32_t count;
  JS_ToUint32(ctx, &address, argv[0]);
  JS_ToUint32(ctx, &count, argv[1]);
  bool stop = true;
  if( argc >= 3 )
    stop = JS_ToBool(ctx, argv[2]);
  return JS_NewUint32(ctx, wire->requestFrom((uint8_t)address, (uint8_t)count, (uint8_t)stop));
}

static JSValue esp32_wire_beginTransmission(JSContext *ctx, JSValueConst jsThis,
                                            int argc, JSValueConst *argv, int magic)
{
  TwoWire *wire;
  if (magic == 0)
    wire = &Wire;
  else if (magic == 1)
    wire = &Wire1;
  else
    return JS_EXCEPTION;

  uint32_t address;
  JS_ToUint32(ctx, &address, argv[0]);
  wire->beginTransmission((uint8_t)address);
  return JS_UNDEFINED;
}

static JSValue esp32_wire_endTransmission(JSContext *ctx, JSValueConst jsThis,
                                          int argc, JSValueConst *argv, int magic)
{
  TwoWire *wire;
  if (magic == 0)
    wire = &Wire;
  else if (magic == 1)
    wire = &Wire1;
  else
    return JS_EXCEPTION;

  bool sendStop = true;
  if (argc > 0)
    sendStop = JS_ToBool(ctx, argv[0]);
  return JS_NewUint32(ctx, wire->endTransmission(sendStop));
}

static JSValue esp32_wire_write(JSContext *ctx, JSValueConst jsThis,
                                int argc, JSValueConst *argv, int magic)
{
  TwoWire *wire;
  if (magic == 0)
    wire = &Wire;
  else if (magic == 1)
    wire = &Wire1;
  else
    return JS_EXCEPTION;

  int tag = JS_VALUE_GET_TAG(argv[0]);
  if (tag == JS_TAG_INT){
    uint32_t value;
    JS_ToUint32(ctx, &value, argv[0]);
    return JS_NewInt32(ctx, wire->write((uint8_t)value));
  }else{
    JSValue jv = JS_GetPropertyStr(ctx, argv[0], "length");
    uint32_t length;
    JS_ToUint32(ctx, &length, jv);
    JS_FreeValue(ctx, jv);

    for (uint32_t i = 0; i < length; i++){
      JSValue jv = JS_GetPropertyUint32(ctx, argv[0], i);
      uint32_t value;
      JS_ToUint32(ctx, &value, jv);
      JS_FreeValue(ctx, jv);
      if (wire->write(value) != 1)
        return JS_EXCEPTION;
    }

    return JS_NewInt32(ctx, length);
  }
}

static JSValue esp32_wire_available(JSContext *ctx, JSValueConst jsThis,
                                    int argc, JSValueConst *argv, int magic)
{
  TwoWire *wire;
  if (magic == 0)
    wire = &Wire;
  else if (magic == 1)
    wire = &Wire1;
  else
    return JS_EXCEPTION;

  return JS_NewInt32(ctx, wire->available());
}

static JSValue esp32_wire_read(JSContext *ctx, JSValueConst jsThis,
                               int argc, JSValueConst *argv, int magic)
{
  TwoWire *wire;
  if (magic == 0)
    wire = &Wire;
  else if (magic == 1)
    wire = &Wire1;
  else
    return JS_EXCEPTION;

  if (argc > 0){
    uint32_t value;
    JS_ToUint32(ctx, &value, argv[0]);
    JSValue array = JS_NewArray(ctx);
    for (uint32_t i = 0; i < value; i++){
      int c = wire->read();
      JS_SetPropertyUint32(ctx, array, i, JS_NewInt32(ctx, c));
    }
    return array;
  }else{
    return JS_NewInt32(ctx, wire->read());
  }
}

static const JSCFunctionListEntry wire_funcs[] = {
    JSCFunctionListEntry{
        "begin", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic_magic, {generic_magic : esp32_wire_begin}}
        }},
    JSCFunctionListEntry{
        "requestFrom", 0, JS_DEF_CFUNC, 0, {
          func : {3, JS_CFUNC_generic_magic, {generic_magic : esp32_wire_requestFrom}}
        }},
    JSCFunctionListEntry{
        "beginTransmission", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic_magic, {generic_magic : esp32_wire_beginTransmission}}
        }},
    JSCFunctionListEntry{
        "endTransmission", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic_magic, {generic_magic : esp32_wire_endTransmission}}
        }},
    JSCFunctionListEntry{
        "write", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic_magic, {generic_magic : esp32_wire_write}}
        }},
    JSCFunctionListEntry{
        "available", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic_magic, {generic_magic : esp32_wire_available}}
        }},
    JSCFunctionListEntry{
        "read", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic_magic, {generic_magic : esp32_wire_read}}
        }},
};

static const JSCFunctionListEntry wire1_funcs[] = {
    JSCFunctionListEntry{
        "begin", 0, JS_DEF_CFUNC, 1, {
          func : {2, JS_CFUNC_generic_magic, {generic_magic : esp32_wire_begin}}
        }},
    JSCFunctionListEntry{
        "requestFrom", 0, JS_DEF_CFUNC, 1, {
          func : {2, JS_CFUNC_generic_magic, {generic_magic : esp32_wire_requestFrom}}
        }},
    JSCFunctionListEntry{
        "beginTransmission", 0, JS_DEF_CFUNC, 1, {
          func : {1, JS_CFUNC_generic_magic, {generic_magic : esp32_wire_beginTransmission}}
        }},
    JSCFunctionListEntry{
        "endTransmission", 0, JS_DEF_CFUNC, 1, {
          func : {1, JS_CFUNC_generic_magic, {generic_magic : esp32_wire_endTransmission}}
        }},
    JSCFunctionListEntry{
        "write", 0, JS_DEF_CFUNC, 1, {
          func : {1, JS_CFUNC_generic_magic, {generic_magic : esp32_wire_write}}
        }},
    JSCFunctionListEntry{
        "available", 0, JS_DEF_CFUNC, 1, {
          func : {0, JS_CFUNC_generic_magic, {generic_magic : esp32_wire_available}}
        }},
    JSCFunctionListEntry{
        "read", 0, JS_DEF_CFUNC, 1, {
          func : {1, JS_CFUNC_generic_magic, {generic_magic : esp32_wire_read}}
        }},
};

JSModuleDef *addModule_wire(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;

  mod = JS_NewCModule(ctx, "Wire", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, wire_funcs,
                            sizeof(wire_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod)
  {
    JS_AddModuleExportList(
        ctx, mod, wire_funcs,
        sizeof(wire_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

JSModuleDef *addModule_wire1(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;

  mod = JS_NewCModule(ctx, "Wire1", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, wire1_funcs,
                            sizeof(wire1_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, wire1_funcs,
        sizeof(wire1_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

JsModuleEntry wire_module = {
  NULL,
  addModule_wire,
  NULL,
  NULL
};

JsModuleEntry wire1_module = {
  NULL,
  addModule_wire1,
  NULL,
  NULL
};