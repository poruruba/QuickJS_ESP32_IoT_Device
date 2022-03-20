#ifndef _ENDPOINT_PACKET_H_
#define _ENDPOINT_PACKET_H_

#include <ArduinoJson.h>
#include "endpoint_types.h"

long packet_initialize(void);
void packet_appendEntry(EndpointEntry *tables, int num_of_entry);
long packet_execute(const char *endpoint, JsonObject params, JsonObject responseResult);

#endif
