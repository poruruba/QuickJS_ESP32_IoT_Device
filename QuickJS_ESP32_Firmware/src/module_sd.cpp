#include <Arduino.h>
#include "quickjs.h"
#include "main_config.h"
#include "module_type.h"
#include "module_utils.h"
#include "module_sd.h"
#include <SD.h>

static JSValue sd_exists(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *fname = JS_ToCString(ctx, argv[0]);
  if( fname == NULL )
    return JS_EXCEPTION;

  bool ret = SD.exists(fname);

  JS_FreeCString(ctx, fname);
  return JS_NewBool(ctx, ret);
}

static JSValue sd_mkdir(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *dname = JS_ToCString(ctx, argv[0]);
  if( dname == NULL )
    return JS_EXCEPTION;

  bool ret = SD.mkdir(dname);

  JS_FreeCString(ctx, dname);
  return JS_NewBool(ctx, ret);
}

static JSValue sd_remove(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *fname = JS_ToCString(ctx, argv[0]);
  if( fname == NULL )
    return JS_EXCEPTION;

  bool ret = SD.remove(fname);

  JS_FreeCString(ctx, fname);
  return JS_NewBool(ctx, ret);
}

static JSValue sd_rmdir(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *dname = JS_ToCString(ctx, argv[0]);
  if( dname == NULL )
    return JS_EXCEPTION;

  bool ret = SD.rmdir(dname);

  JS_FreeCString(ctx, dname);
  return JS_NewBool(ctx, ret);
}

static JSValue sd_size(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *fname = JS_ToCString(ctx, argv[0]);
  if( fname == NULL )
    return JS_EXCEPTION;
  File file = SD.open(fname);
  JS_FreeCString(ctx, fname);
  if( !file )
    return JS_EXCEPTION;

  uint32_t size = file.size();

  file.close();

  return JS_NewUint32(ctx, size);
}

static JSValue sd_readText(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *fname = JS_ToCString(ctx, argv[0]);
  if( fname == NULL )
    return JS_EXCEPTION;

  bool sem = xSemaphoreTake(binSem, portMAX_DELAY);
  File file = SD.open(fname, FILE_READ);
  JS_FreeCString(ctx, fname);
  if( !file ){
    if( sem ) xSemaphoreGive(binSem);
    return JS_EXCEPTION;
  }

  uint32_t fsize = file.size();
  char *p_buffer = (char*)malloc(fsize + 1);
  if( p_buffer == NULL ){
    file.close();
    if( sem ) xSemaphoreGive(binSem);
    return JS_EXCEPTION;
  }
  file.readBytes(p_buffer, fsize);
  file.close();
  if( sem ) xSemaphoreGive(binSem);
  p_buffer[fsize] = '\0';
  JSValue value = JS_NewString(ctx, p_buffer);
  free(p_buffer);

  return value;
}

static JSValue sd_writeText(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv, int magic)
{
  const char *fname = JS_ToCString(ctx, argv[0]);
  if( fname == NULL )
    return JS_EXCEPTION;

  bool sem = xSemaphoreTake(binSem, portMAX_DELAY);
  File file = SD.open(fname, FILE_WRITE);
  JS_FreeCString(ctx, fname);
  if( !file ){
    if( sem ) xSemaphoreGive(binSem);
    return JS_EXCEPTION;
  }

  const char *str = JS_ToCString(ctx, argv[1]);
  if( str == NULL ){
    file.close();
    if( sem ) xSemaphoreGive(binSem);
    return JS_EXCEPTION;
  }
  size_t size = strlen(str);
  if( magic == 1 )
    file.seek(file.size());
  size_t wrote = file.write((const uint8_t*)str, size);
  file.close();
  if( sem ) xSemaphoreGive(binSem);

  JS_FreeCString(ctx, str);
  if( wrote != size )
    return JS_EXCEPTION;

  return JS_NewInt32(ctx, wrote);
}

static JSValue sd_readBinary(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *fname = JS_ToCString(ctx, argv[0]);
  if( fname == NULL )
    return JS_EXCEPTION;

  bool sem = xSemaphoreTake(binSem, portMAX_DELAY);
  File file = SD.open(fname, FILE_READ);
  JS_FreeCString(ctx, fname);
  if( !file ){
    if( sem ) xSemaphoreGive(binSem);
    return JS_EXCEPTION;
  }

  uint32_t fsize = file.size();
  int32_t offset = 0;
  if( argc >= 2 )
    JS_ToInt32(ctx, &offset, argv[1]);
  if( offset < 0 )
    offset = 0;
  if( offset > fsize )
    offset = fsize;
  int32_t size = -1;
  if( argc >= 3)
    JS_ToInt32(ctx, &size, argv[2]);
  if( size < 0 || offset + size > fsize )
    size = fsize - offset;

  unsigned char *buffer = (unsigned char*)malloc(size);
  if( buffer == NULL ){
    file.close();
    if( sem ) xSemaphoreGive(binSem);
    return JS_EXCEPTION;
  }
  file.seek(offset);
  file.read(buffer, size);
  file.close();
  if( sem ) xSemaphoreGive(binSem);

  JSValue value = JS_NewArrayBufferCopy(ctx, buffer, size);
  free(buffer);

  return value;
}

static JSValue sd_writeBinary(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *fname = JS_ToCString(ctx, argv[0]);
  if( fname == NULL )
    return JS_EXCEPTION;

  bool sem = xSemaphoreTake(binSem, portMAX_DELAY);
  File file = SD.open(fname, FILE_WRITE);
  JS_FreeCString(ctx, fname);
  if( !file ){
    if( sem ) xSemaphoreGive(binSem);
    return JS_EXCEPTION;
  }

  uint8_t *p_buffer;
  uint8_t unit_size;
  uint32_t bsize;
  JSValue vbuffer = getArrayBuffer(ctx, argv[1], (void**)&p_buffer, &unit_size, &bsize);
  if( JS_IsNull(vbuffer) ){
    file.close();
    if( sem ) xSemaphoreGive(binSem);
    return JS_EXCEPTION;
  }
  if( unit_size != 1 ){
    file.close();
    if( sem ) xSemaphoreGive(binSem);
    JS_FreeValue(ctx, vbuffer);
      return JS_EXCEPTION;
  }

  uint32_t fsize = file.size();
  int32_t offset = -1;
  if( argc >= 3 )
    JS_ToInt32(ctx, &offset, argv[2]);
  if( offset < 0 )
    offset = fsize;
  int32_t size = -1;
  if( argc >= 4)
    JS_ToInt32(ctx, &size, argv[3]);
  if( size < 0 || size > bsize )
    size = bsize;

  file.seek(offset);
  size_t wrote = file.write(p_buffer, size);
  file.close();
  if( sem ) xSemaphoreGive(binSem);

  JS_FreeValue(ctx, vbuffer);

  return JS_NewInt32(ctx, wrote);
}

static JSValue sd_isDirectory(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *fname = JS_ToCString(ctx, argv[0]);
  if( fname == NULL )
    return JS_EXCEPTION;
  File file = SD.open(fname, FILE_READ);
  JS_FreeCString(ctx, fname);
  if( !file )
    return JS_EXCEPTION;
  
  bool ret = file.isDirectory();
  file.close();

  return JS_NewBool(ctx, ret);
}

static JSValue sd_list(JSContext *ctx, JSValueConst jsThis, int argc, JSValueConst *argv)
{
  const char *fname = JS_ToCString(ctx, argv[0]);
  if( fname == NULL )
    return JS_EXCEPTION;
  File base = SD.open(fname, FILE_READ);
  JS_FreeCString(ctx, fname);
  if( !base )
    return JS_EXCEPTION;

  JSValue jsArray = JS_NewArray(ctx);
  File file;
  for( int i = 0 ; file = base.openNextFile(); i++ ){
    JS_SetPropertyUint32(ctx, jsArray, i, JS_NewString(ctx, file.name()));
    file.close();
  }
  base.close();

  return jsArray;
}

static const JSCFunctionListEntry sd_funcs[] = {
    JSCFunctionListEntry{
        "exists", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, sd_exists}
        }},
    JSCFunctionListEntry{
        "mkdir", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, sd_mkdir}
        }},
    JSCFunctionListEntry{
        "remove", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, sd_remove}
        }},
    JSCFunctionListEntry{
        "rmdir", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, sd_rmdir}
        }},
    JSCFunctionListEntry{
        "size", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, sd_size}
        }},
    JSCFunctionListEntry{
        "readBinary", 0, JS_DEF_CFUNC, 0, {
          func : {3, JS_CFUNC_generic, sd_readBinary}
        }},
    JSCFunctionListEntry{
        "writeBinary", 0, JS_DEF_CFUNC, 0, {
          func : {4, JS_CFUNC_generic, sd_writeBinary}
        }},
    JSCFunctionListEntry{
        "readText", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, sd_readText}
        }},
    JSCFunctionListEntry{
        "writeText", 0, JS_DEF_CFUNC, 0, {
            func : {2, JS_CFUNC_generic_magic, {generic_magic : sd_writeText }}
        }},
    JSCFunctionListEntry{
        "appendText", 0, JS_DEF_CFUNC, 1, {
            func : {2, JS_CFUNC_generic_magic, {generic_magic : sd_writeText }}
        }},
    JSCFunctionListEntry{
        "isDirectory", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, sd_isDirectory}
        }},
    JSCFunctionListEntry{
        "list", 0, JS_DEF_CFUNC, 0, {
          func : {1, JS_CFUNC_generic, sd_list}
        }},
};

JSModuleDef *addModule_sd(JSContext *ctx, JSValue global)
{
  JSModuleDef *mod;

  mod = JS_NewCModule(ctx, "Sd", [](JSContext *ctx, JSModuleDef *m) {
          return JS_SetModuleExportList(
              ctx, m, sd_funcs,
              sizeof(sd_funcs) / sizeof(JSCFunctionListEntry));
        });
  if (mod) {
    JS_AddModuleExportList(
        ctx, mod, sd_funcs,
        sizeof(sd_funcs) / sizeof(JSCFunctionListEntry));
  }

  return mod;
}

long initialize_sd(void)
{
  bool ret = SD.begin();

  return ret ? 0 : -1;
}

JsModuleEntry sd_module = {
  initialize_sd,
  addModule_sd,
  NULL,
  NULL
};