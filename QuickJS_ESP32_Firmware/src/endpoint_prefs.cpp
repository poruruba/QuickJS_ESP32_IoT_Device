#include <Arduino.h>

#include "endpoint_types.h"
#include "module_prefs.h"
#include "module_utils.h"
#include "endpoint_prefs.h"

long endp_prefs_remove(JsonObject request, JsonObject response, int magic)
{
  const char *name = request["name"];
  if( name == NULL )
    return -1;
  const char *key = request["key"];
  if( key == NULL )
    return -1;

  if( preferences._started )
    return -1;
  if( !preferences.begin(name, false) )
    return -1;
  bool ret = preferences.remove(key);
  preferences.end();

  response["result"] = ret;

  return 0;
}

long endp_prefs_isKey(JsonObject request, JsonObject response, int magic)
{
  const char *name = request["name"];
  if( name == NULL )
    return -1;
  const char *key = request["key"];
  if( key == NULL )
    return -1;

  if( preferences._started )
    return -1;
  if( !preferences.begin(name, true) )
    return -1;
  bool ret = preferences.isKey(key);
  preferences.end();

  response["result"] = ret;

  return 0;
}

long endp_prefs_getType(JsonObject request, JsonObject response, int magic)
{
  const char *name = request["name"];
  if( name == NULL )
    return -1;
  const char *key = request["key"];
  if( key == NULL )
    return -1;

  if( preferences._started )
    return -1;
  if( !preferences.begin(name, true) )
    return -1;
  uint32_t ret = preferences.getType(key);
  preferences.end();

  response["result"] = ret;

  return 0;
}

long endp_prefs_clear(JsonObject request, JsonObject response, int magic)
{
  const char *name = request["name"];
  if( name == NULL )
    return -1;

  if( preferences._started )
    return -1;

  if( !preferences.begin(name, false) )
    return -1;
  bool ret = preferences.clear();
  preferences.end();

  response["result"] = ret;

  return 0;
}

long endp_prefs_putNumber(JsonObject request, JsonObject response, int magic)
{
  const char *name = request["name"];
  if( name == NULL )
    return -1;
  const char *key = request["key"];
  if( key == NULL )
    return -1;
  int32_t value = request["value"];

  if( preferences._started )
    return -1;
  if( !preferences.begin(name, false) )
    return -1;
  size_t ret = preferences.putLong(key, value);
  preferences.end();

  response["result"] = ret;

  return 0;
}

long endp_prefs_getNumber(JsonObject request, JsonObject response, int magic)
{
  const char *name = request["name"];
  if( name == NULL )
    return -1;
  const char *key = request["key"];
  if( key == NULL )
    return -1;
  int32_t def = request["def"] || 0;

  if( preferences._started )
    return -1;
  if( !preferences.begin(name, true) )
    return -1;
  int32_t ret = preferences.getLong(key, def);
  preferences.end();

  response["result"] = ret;

  return 0;
}

long endp_prefs_putDouble(JsonObject request, JsonObject response, int magic)
{
  const char *name = request["name"];
  if( name == NULL )
    return -1;
  const char *key = request["key"];
  if( key == NULL )
    return -1;
  double value = request["value"];

  if( preferences._started )
    return -1;
  if( !preferences.begin(name, false) )
    return -1;
  size_t ret = preferences.putDouble(key, value);
  preferences.end();

  response["result"] = ret;

  return 0;
}

long endp_prefs_getDouble(JsonObject request, JsonObject response, int magic)
{
  const char *name = request["name"];
  if( name == NULL )
    return -1;
  const char *key = request["key"];
  if( key == NULL )
    return -1;
  double def = request["def"] || 0.0;

  if( preferences._started )
    return -1;
  if( !preferences.begin(name, true) )
    return -1;
  double ret = preferences.getDouble(key, def);
  preferences.end();

  response["result"] = ret;

  return 0;
}

long endp_prefs_putBool(JsonObject request, JsonObject response, int magic)
{
  const char *name = request["name"];
  if( name == NULL )
    return -1;
  const char *key = request["key"];
  if( key == NULL )
    return -1;
  bool value = request["value"];

  if( preferences._started )
    return -1;
  if( !preferences.begin(name, false) )
    return -1;
  size_t ret = preferences.putBool(key, value);
  preferences.end();

  response["result"] = ret;

  return 0;
}

long endp_prefs_getBool(JsonObject request, JsonObject response, int magic)
{
  const char *name = request["name"];
  if( name == NULL )
    return -1;
  const char *key = request["key"];
  if( key == NULL )
    return -1;
  bool def = request["def"] || false;

  if( preferences._started )
    return -1;
  if( !preferences.begin(name, true) )
    return -1;
  bool ret = preferences.getBool(key, def);
  preferences.end();

  response["result"] = ret;

  return 0;
}

long endp_prefs_putString(JsonObject request, JsonObject response, int magic)
{
  const char *name = request["name"];
  if( name == NULL )
    return -1;
  const char *key = request["key"];
  if( key == NULL )
    return -1;
  const char *value = request["value"];
  if( value == NULL )
    return -1;

  if( preferences._started )
    return -1;
  if( !preferences.begin(name, false) )
    return -1;
  size_t ret = preferences.putString(key, value);
  preferences.end();

  response["result"] = ret;

  return 0;
}

long endp_prefs_getString(JsonObject request, JsonObject response, int magic)
{
  const char *name = request["name"];
  if( name == NULL )
    return -1;
  const char *key = request["key"];
  if( key == NULL )
    return -1;

  if( preferences._started )
    return -1;
  if( !preferences.begin(name, true) )
    return -1;
  String ret = preferences.getString(key);
  preferences.end();

  response["result"] = (char*)ret.c_str();

  return 0;
}

long endp_prefs_putBytes(JsonObject request, JsonObject response, int magic)
{
  const char *name = request["name"];
  if( name == NULL )
    return -1;
  const char *key = request["key"];
  if( key == NULL )
    return -1;
  const char *encstr = request["value"];
  if( encstr == NULL )
    return -1;

  if( preferences._started )
    return -1;
  if( !preferences.begin(name, false) )
    return -1;

  uint32_t bsize = b64_decode_length(encstr);
  unsigned char *value = (unsigned char*)malloc(bsize);
  b64_decode(encstr, value);
  size_t ret = preferences.putBytes(key, value, bsize);
  free(value);

  preferences.end();

  response["result"] = ret;

  return 0;
}

long endp_prefs_getBytes(JsonObject request, JsonObject response, int magic)
{
  const char *name = request["name"];
  if( name == NULL )
    return -1;
  const char *key = request["key"];
  if( key == NULL )
    return -1;
  uint32_t def = request["def"] || PREF_MAX_BYTES_LEN;

  if( preferences._started )
    return -1;
  if( !preferences.begin(name, true) )
    return -1;

  uint8_t *p_buffer = (uint8_t*)malloc(def);
  if( p_buffer == NULL )
    return -1;

  size_t ret = preferences.getBytes(key, p_buffer, def);
  preferences.end();

  uint32_t enclen = b64_encode_length(ret);
  char *encstr = (char*)malloc(enclen + 1);
  if( encstr == NULL ){
    free(p_buffer);
    return -1;
  }
  b64_encode(p_buffer, ret, encstr);
  encstr[enclen] = '\0';
  free(p_buffer);

  response["result"] = encstr;

  free(encstr);

  return 0;
}

EndpointEntry prefs_table[] = {
  EndpointEntry{ endp_prefs_clear, "/prefs-clear", -1 },
  EndpointEntry{ endp_prefs_remove, "/prefs-remove", -1 },
  EndpointEntry{ endp_prefs_isKey, "/prefs-isKey", -1 },
  EndpointEntry{ endp_prefs_getType, "/prefs-getType", -1 },
  EndpointEntry{ endp_prefs_putNumber, "/prefs-putNumber", -1 },
  EndpointEntry{ endp_prefs_getNumber, "/prefs-getNumber", -1 },
  EndpointEntry{ endp_prefs_putDouble, "/prefs-putDouble", -1 },
  EndpointEntry{ endp_prefs_getDouble, "/prefs-getDouble", -1 },
  EndpointEntry{ endp_prefs_putBool, "/prefs-putBool", -1 },
  EndpointEntry{ endp_prefs_getBool, "/prefs-getBool", -1 },
  EndpointEntry{ endp_prefs_putString, "/prefs-putString", -1 },
  EndpointEntry{ endp_prefs_getString, "/prefs-getString", -1 },
  EndpointEntry{ endp_prefs_putBytes, "/prefs-putBytes", -1 },
  EndpointEntry{ endp_prefs_getBytes, "/prefs-getBytes", -1 },
};

const int num_of_prefs_entry = sizeof(prefs_table) / sizeof(EndpointEntry);
