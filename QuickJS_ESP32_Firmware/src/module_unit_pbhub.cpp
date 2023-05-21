#include <Arduino.h>
#include "main_config.h"

#ifdef _UNIT_PBHUB_ENABLE_

#include "quickjs.h"
#include "module_unit_pbhub.h"
#include "porthub.h"

static PortHub porthub;
static uint8_t HUB_ADDR[6] = {HUB1_ADDR, HUB2_ADDR, HUB3_ADDR, HUB4_ADDR, HUB5_ADDR, HUB6_ADDR};

#define PBHUB_PORT_A  0
#define PBHUB_PORT_B  1

static JSValue unit_pbhub_begin(JSContext *ctx, JSValueConst jsThis,
                                      int argc, JSValueConst *argv)
{
  porthub.begin();

  return JS_UNDEFINED;
}

static JSValue unit_pbhub_analogRead(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t reg;
  JS_ToUint32(ctx, &reg, argv[0]);
  if( reg >= sizeof(HUB_ADDR))
    return JS_EXCEPTION;

  uint16_t value = porthub.hub_a_read_value(HUB_ADDR[reg]);

  return JS_NewUint32(ctx, value);
}

static JSValue unit_pbhub_digitalRead(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t reg, port;
  JS_ToUint32(ctx, &reg, argv[0]);
  JS_ToUint32(ctx, &port, argv[1]);
  if( reg >= sizeof(HUB_ADDR))
    return JS_EXCEPTION;

  uint8_t value;
  if( port == PBHUB_PORT_A )
    value = porthub.hub_d_read_value_A(HUB_ADDR[reg]);
  else if( port == PBHUB_PORT_B )
    value = porthub.hub_d_read_value_B(HUB_ADDR[reg]);
  else
    return JS_EXCEPTION;

  return JS_NewUint32(ctx, value);
}

static JSValue unit_pbhub_analogWrite(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t reg, port, duty;
  JS_ToUint32(ctx, &reg, argv[0]);
  JS_ToUint32(ctx, &port, argv[1]);
  JS_ToUint32(ctx, &duty, argv[2]);
  if( reg >= sizeof(HUB_ADDR))
    return JS_EXCEPTION;

  if( port == PBHUB_PORT_A )
    porthub.hub_a_wire_value_A(HUB_ADDR[reg], duty);
  else if( port == PBHUB_PORT_B )
    porthub.hub_a_wire_value_B(HUB_ADDR[reg], duty);
  else
    return JS_EXCEPTION;

  return JS_UNDEFINED;
}

static JSValue unit_pbhub_digitalWrite(JSContext *ctx, JSValueConst jsThis,
                                     int argc, JSValueConst *argv)
{
  uint32_t reg, port, level;
  JS_ToUint32(ctx, &reg, argv[0]);
  JS_ToUint32(ctx, &port, argv[1]);
  JS_ToUint32(ctx, &level, argv[2]);
  if( reg >= sizeof(HUB_ADDR))
    return JS_EXCEPTION;

  if( port == PBHUB_PORT_A )
    porthub.hub_d_wire_value_A(HUB_ADDR[reg], level);
  else if( port == PBHUB_PORT_B )
    porthub.hub_d_wire_value_B(HUB_ADDR[reg], level);
  else
    return JS_EXCEPTION;

  return JS_UNDEFINED;
}

static const JSCFunctionListEntry unit_pbhub_funcs[] = {
    JSCFunctionListEntry{
        "begin", 0, JS_DEF_CFUNC, 0, {
          func : {0, JS_CFUNC_generic, unit_pbhub_begin}
        }},
    JSCFunctionListEntry{
        "analogRead", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, unit_pbhub_analogRead}
        }},
    JSCFunctionListEntry{
        "digitalRead", 0, JS_DEF_CFUNC, 0, {
          func : {2, JS_CFUNC_generic, unit_pbhub_digitalRead}
        }},
    JSCFunctionListEntry{
        "analogWrite", 0, JS_DEF_CFUNC, 0, {
          func : {3, JS_CFUNC_generic, unit_pbhub_analogWrite}
        }},
    JSCFunctionListEntry{
        "digitalWrite", 0, JS_DEF_CFUNC, 0, {
          func : {3, JS_CFUNC_generic, unit_pbhub_digitalWrite}
        }},
    JSCFunctionListEntry{
        "PORT_A", 0, JS_DEF_PROP_INT32, 0, {
          i32 : PBHUB_PORT_A
        }},
    JSCFunctionListEntry{
        "PORT_B", 0, JS_DEF_PROP_INT32, 0, {
          i32 : PBHUB_PORT_B
        }},
};

JSModuleDef *addModule_unit_pbhub(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;
  mod = JS_NewCModule(ctx, "UnitPbhub", [](JSContext *ctx, JSModuleDef *m)
                      { return JS_SetModuleExportList(
                            ctx, m, unit_pbhub_funcs,
                            sizeof(unit_pbhub_funcs) / sizeof(JSCFunctionListEntry)); });
  if (mod){
    JS_AddModuleExportList(
        ctx, mod, unit_pbhub_funcs,
        sizeof(unit_pbhub_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

JsModuleEntry unit_pbhub_module = {
  NULL,
  addModule_unit_pbhub,
  NULL,
  NULL
};

#endif
