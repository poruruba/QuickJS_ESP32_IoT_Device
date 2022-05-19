#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>
#include <unordered_map> 
#include "module_utils.h"

#include "main_config.h"
#include "endpoint_types.h"
#include "endpoint_packet.h"

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>

#include "endpoint_esp32.h"
#include "endpoint_gpio.h"
#include "endpoint_ledc.h"
#include "endpoint_wire.h"
#include "endpoint_prefs.h"
#ifdef _RTC_ENABLE_
#include "endpoint_rtc.h"
#endif
#ifdef _IMU_ENABLE_
#include "endpoint_imu.h"
#endif
#ifdef _SD_ENABLE_
#include "endpoint_sd.h"
#endif
#ifdef _LCD_ENABLE_
#include "endpoint_lcd.h"
#endif

static AsyncWebServer server(HTTP_PORT);

static std::unordered_map<std::string, EndpointEntry*> endpoint_list;

void packet_appendEntry(EndpointEntry *tables, int num_of_entry)
{
  for(int i = 0 ; i < num_of_entry ; i++ )
    endpoint_list[tables[i].name] = &tables[i];
}

long packet_execute(const char *endpoint, JsonObject params, JsonObject responseResult)
{
  std::unordered_map<std::string, EndpointEntry*>::iterator itr = endpoint_list.find(endpoint);
  if( itr != endpoint_list.end() ){
    EndpointEntry *entry = itr->second;
    long ret = entry->impl(params, responseResult, entry->magic);
    return ret;
  }

  Serial.println("endpoint not found");
  return -1;
}

static void notFound(AsyncWebServerRequest *request)
{
  if (request->method() == HTTP_OPTIONS){
    request->send(200);
  }else{
    request->send(404);
  }
}

long packet_initialize(void)
{
  if( !is_wifi_connected() )
    return -1;

  if (!MDNS.begin(MDNS_NAME)){
    Serial.println("MDNS.begin error");
  }else{
    Serial.printf("MDNS_NAME: %s\n", MDNS_NAME);
    MDNS.addService("http", "tcp", HTTP_PORT);
    Serial.printf("serivce_name: %s, TCP_PORT: %d\n", "http", HTTP_PORT);
  }

  packet_appendEntry(esp32_table, num_of_esp32_entry);
  packet_appendEntry(gpio_table, num_of_gpio_entry);
  packet_appendEntry(wire_table, num_of_wire_entry);
  packet_appendEntry(ledc_table, num_of_ledc_entry);
  packet_appendEntry(prefs_table, num_of_prefs_entry);
#ifdef _RTC_ENABLE_
  packet_appendEntry(rtc_table, num_of_rtc_entry);
#endif
#ifdef _IMU_ENABLE_
  packet_appendEntry(imu_table, num_of_imu_entry);
#endif
#ifdef _SD_ENABLE_
  packet_appendEntry(sd_table, num_of_sd_entry);
#endif
#ifdef _LCD_ENABLE_
  packet_appendEntry(lcd_table, num_of_lcd_entry);
#endif

  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/endpoint", [](AsyncWebServerRequest *request, JsonVariant &json) {
    JsonObject jsonObj = json.as<JsonObject>();
    const char *endpoint = jsonObj["endpoint"];
    AsyncJsonResponse *response = new AsyncJsonResponse(false, PACKET_JSON_DOCUMENT_SIZE);
    JsonObject responseResult = response->getRoot();
    responseResult["status"] = "OK";
    responseResult["endpoint"] = (char*)endpoint;
    bool sem = xSemaphoreTake(binSem, portMAX_DELAY);
    long ret = packet_execute(endpoint, jsonObj["params"], responseResult);
    if( sem )
      xSemaphoreGive(binSem);
    if( ret != 0 ){
      responseResult.clear();
      responseResult["status"] = "NG";
      responseResult["endpoint"] = (char*)endpoint;
      responseResult["message"] = "unknown";
    }
    response->setLength();
    request->send(response);
  });
  server.addHandler(handler);

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
//  server.serveStatic("/", SPIFFS, "/html/").setDefaultFile("index.html");
  server.onNotFound(notFound);
  server.begin();

  return 0;
}
