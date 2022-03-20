#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>
#include <unordered_map> 
#include "module_utils.h"

#include "main_config.h"
#include "endpoint_types.h"
#include "endpoint_packet.h"

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

  return 0;
}
