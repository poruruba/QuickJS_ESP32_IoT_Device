#include <Arduino.h>
#include "esp32-hal-ledc.h"

#include "endpoint_types.h"
#include "endpoint_ledc.h"

long endp_ledc_setup(JsonObject request, JsonObject response, int magic)
{
  uint8_t channel = request["channel"];
  float freq = request["freq"];
  uint8_t resolution = request["resolution"];

  float ret = ledcSetup(channel, freq, resolution);
  response["result"] = ret;

  return 0;
}

long endp_ledc_attachPin(JsonObject request, JsonObject response, int magic)
{
  uint8_t pin = request["pin"];
  uint8_t channel = request["channel"];

  ledcAttachPin(pin, channel);

  return 0;
}

long endp_ledc_detachPin(JsonObject request, JsonObject response, int magic)
{
  uint8_t pin = request["pin"];

  ledcDetachPin(pin);

  return 0;
}

long endp_ledc_write(JsonObject request, JsonObject response, int magic)
{
  uint8_t channel = request["channel"];
  uint32_t duty = request["duty"];

  ledcWrite(channel, duty);

  return 0;
}

long endp_ledc_writeTone(JsonObject request, JsonObject response, int magic)
{
  uint8_t channel = request["channel"];
  float freq = request["freq"];

  float ret = ledcWriteTone(channel, freq);
  response["result"] = ret;

  return 0;
}

long endp_ledc_writeNote(JsonObject request, JsonObject response, int magic)
{
  uint8_t channel = request["channel"];
  uint8_t note = request["note"];
  uint8_t octave = request["octave"];

  float ret = ledcWriteNote(channel, (note_t)note, octave);
  response["result"] = ret;

  return 0;
}

long endp_ledc_read(JsonObject request, JsonObject response, int magic)
{
  uint8_t channel = request["channel"];

  uint32_t ret = ledcRead(channel);
  response["result"] = ret;

  return 0;
}

long endp_ledc_readFreq(JsonObject request, JsonObject response, int magic)
{
  uint8_t channel = request["channel"];

  float ret = ledcReadFreq(channel);
  response["result"] = ret;

  return 0;
}

EndpointEntry ledc_table[] = {
  EndpointEntry{ endp_ledc_setup, "/ledc-setup", -1 },
  EndpointEntry{ endp_ledc_attachPin, "/ledc-attachPin", -1 },
  EndpointEntry{ endp_ledc_detachPin, "/ledc-detachPin", -1 },
  EndpointEntry{ endp_ledc_write, "/ledc-write", -1 },
  EndpointEntry{ endp_ledc_writeTone, "/ledc-writeTone", -1 },
  EndpointEntry{ endp_ledc_writeNote, "/ledc-writeNote", -1 },
  EndpointEntry{ endp_ledc_read, "/ledc-read", -1 },
  EndpointEntry{ endp_ledc_readFreq, "/ledc-readFreq", -1 },
};

const int num_of_ledc_entry = sizeof(ledc_table) / sizeof(EndpointEntry);
