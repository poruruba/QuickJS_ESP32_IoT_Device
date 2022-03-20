#include <Arduino.h>

#include "endpoint_types.h"
#include "endpoint_gpio.h"

long endp_gpio_pinMode(JsonObject request, JsonObject response, int magic)
{
  uint8_t pin = request["pin"];
  uint8_t mode = request["mode"];

  pinMode(pin, mode);

  return 0;
}

long endp_gpio_analogRead(JsonObject request, JsonObject response, int magic)
{
  uint8_t pin = request["pin"];

  uint16_t ret = analogRead(pin);

  response["result"] = ret;

  return 0;
}

long endp_gpio_digitalRead(JsonObject request, JsonObject response, int magic)
{
  uint8_t pin = request["pin"];

  int ret = digitalRead(pin);

  response["result"] = ret;

  return 0;
}

long endp_gpio_digitalWrite(JsonObject request, JsonObject response, int magic)
{
  uint8_t pin = request["pin"];
  uint8_t value = request["value"];

  digitalWrite(pin, value);

  return 0;
}

EndpointEntry gpio_table[] = {
  EndpointEntry{ endp_gpio_pinMode, "/gpio-pinMode", -1 },
  EndpointEntry{ endp_gpio_analogRead, "/gpio-analogRead", -1 },
  EndpointEntry{ endp_gpio_digitalRead, "/gpio-digitalRead", -1 },
  EndpointEntry{ endp_gpio_digitalWrite, "/gpio-digitalWrite", -1 }
};

const int num_of_gpio_entry = sizeof(gpio_table) / sizeof(EndpointEntry);
