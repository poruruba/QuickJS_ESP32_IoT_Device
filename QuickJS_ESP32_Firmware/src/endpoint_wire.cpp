#include <Arduino.h>
#include <Wire.h>

#include "endpoint_types.h"
#include "endpoint_wire.h"

long endp_wire_begin(JsonObject request, JsonObject response, int magic)
{
  TwoWire *wire;
  if (magic == 0)
    wire = &Wire;
  else if (magic == 1)
    wire = &Wire1;
  else
    return -1;
    
  int sda = request["sda"] | -1;
  int scl = request["scl"] | -1;
  uint32_t freq = request["freq"] | 0;

  bool ret = wire->begin(sda, scl, freq);

  response["result"] = ret;

  return 0;
}

long endp_wire_requestFrom(JsonObject request, JsonObject response, int magic)
{
  TwoWire *wire;
  if (magic == 0)
    wire = &Wire;
  else if (magic == 1)
    wire = &Wire1;
  else
    return -1;

  uint8_t address = request["address"];
  uint8_t count = request["count"];
  bool stop = request["stop"] | true;

  uint8_t ret = wire->requestFrom(address, count, (uint8_t)stop);
  response["result"] = ret;

  return 0;
}

long endp_wire_beginTransmission(JsonObject request, JsonObject response, int magic)
{
  TwoWire *wire;
  if (magic == 0)
    wire = &Wire;
  else if (magic == 1)
    wire = &Wire1;
  else
    return -1;

  uint8_t address = request["address"];

  wire->beginTransmission(address);

  return 0;
}

long endp_wire_endTransmission(JsonObject request, JsonObject response, int magic)
{
  TwoWire *wire;
  if (magic == 0)
    wire = &Wire;
  else if (magic == 1)
    wire = &Wire1;
  else
    return -1;

  bool stop = request["stop"] | true;

  uint8_t ret = wire->endTransmission(stop);
  response["result"] = ret;

  return 0;
}

long endp_wire_write(JsonObject request, JsonObject response, int magic)
{
  TwoWire *wire;
  if (magic == 0)
    wire = &Wire;
  else if (magic == 1)
    wire = &Wire1;
  else
    return -1;

  if( request["value"].is<JsonArray>() ){
    JsonArray arry = request["value"];
    int size = arry.size();
    for( int i = 0 ; i < size ; i++ ){
      uint8_t value = arry[i];
      if( wire->write(value) != 1 )
        return -1;
    }

    response["result"] = size;
  }else{
    uint8_t value = request["value"];
    int ret = wire->write(value);
    response["result"] = ret;
  }

  return 0;
}

long endp_wire_available(JsonObject request, JsonObject response, int magic)
{
  TwoWire *wire;
  if (magic == 0)
    wire = &Wire;
  else if (magic == 1)
    wire = &Wire1;
  else
    return -1;
  
  int ret = wire->available();

  response["result"] = ret;

  return 0;
}

long endp_wire_read(JsonObject request, JsonObject response, int magic)
{
  TwoWire *wire;
  if (magic == 0)
    wire = &Wire;
  else if (magic == 1)
    wire = &Wire1;
  else
    return -1;

  if( request.containsKey("count") ){
    int count = request["count"];
    JsonArray arry = response.createNestedArray("result");
    for( int i = 0 ; i < count ; i++ )
      arry[i] = wire->read();
  }else{
    response["result"] = wire->read();
  }

  return 0;
}

EndpointEntry wire_table[] = {
  EndpointEntry{ endp_wire_begin, "/wire-begin", 0 },
  EndpointEntry{ endp_wire_requestFrom, "/wire-requestFrom", 0 },
  EndpointEntry{ endp_wire_beginTransmission, "/wire-beginTransmission", 0 },
  EndpointEntry{ endp_wire_endTransmission, "/wire-endTransmission", 0 },
  EndpointEntry{ endp_wire_write, "/wire-write", 0 },
  EndpointEntry{ endp_wire_available, "/wire-available", 0 },
  EndpointEntry{ endp_wire_read, "/wire-read", 0 },

  EndpointEntry{ endp_wire_begin, "/wire1-begin", 1 },
  EndpointEntry{ endp_wire_requestFrom, "/wire1-requestFrom", 1 },
  EndpointEntry{ endp_wire_beginTransmission, "/wire1-beginTransmission", 1 },
  EndpointEntry{ endp_wire_endTransmission, "/wire1-endTransmission", 1 },
  EndpointEntry{ endp_wire_write, "/wire1-write", 1 },
  EndpointEntry{ endp_wire_available, "/wire1-available", 1 },
  EndpointEntry{ endp_wire_read, "/wire1-read", 1 }
};

const int num_of_wire_entry = sizeof(wire_table) / sizeof(EndpointEntry);
