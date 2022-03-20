#ifndef _ENDPOINT_TYPES_H_
#define _ENDPOINT_TYPES_H_

#include <ArduinoJson.h>

typedef long(*EndpointImpl)(JsonObject, JsonObject, int magic);

typedef struct {
  EndpointImpl impl;
  const char *name;
  int magic;
} EndpointEntry;

#endif