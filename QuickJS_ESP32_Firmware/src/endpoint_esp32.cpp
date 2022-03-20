#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>

#include "main_config.h"
#include "endpoint_packet.h"
#include "endpoint_types.h"
#include "endpoint_esp32.h"

long endp_putText(JsonObject request, JsonObject response, int magic)
{
  const char *p_text = request["text"];
  if( (strlen(p_text) + 1) > FILE_BUFFER_SIZE )
    return -1;

  strcpy(g_download_buffer, p_text);
  g_fileloading = FILE_LOADING_TEXT;

  return 0;
}

long endp_restart(JsonObject request, JsonObject response, int magic)
{
  g_download_buffer[0] = '\0';
  g_fileloading = FILE_LOADING_RESTART;

  return 0;
}

long endp_reboot(JsonObject request, JsonObject response, int magic)
{
  g_download_buffer[0] = '\0';
  g_fileloading = FILE_LOADING_REBOOT;
  return 0;
}

long endp_millis(JsonObject request, JsonObject response, int magic)
{
  response["result"] = millis();
  return 0;
}

long endp_getIpAddress(JsonObject request, JsonObject response, int magic)
{
  IPAddress address = WiFi.localIP();
  response["result"] = (uint32_t)(((uint32_t)address[0]) << 24 | address[1] << 16 | address[2] << 8 | address[3]);

  return 0;
}

long endp_getMacAddress(JsonObject request, JsonObject response, int magic)
{
  uint8_t address[6];
  WiFi.macAddress(address);
  for( int i = 0 ; i < 6 ; i++ )
    response["result"][i] = address[i];

  return 0;
}

long endp_code_upload(JsonObject request, JsonObject response, int magic)
{
  const char *p_code = request["code"];
  const char *p_fname = request["fname"];

  if( p_fname == NULL ){
    if( (strlen(p_code) + 1) > sizeof(g_download_buffer) )
      return -1;
    strcpy( g_download_buffer, p_code);
    g_fileloading = FILE_LOADING_JS;
  }else{
    long ret = save_module(p_fname, p_code);
    if( ret != 0 )
      return 0;
  }

  return 0;
}

long endp_code_download(JsonObject request, JsonObject response, int magic)
{
  const char *p_fname = request["fname"];

  g_fileloading = FILE_LOADING_NONE;
  if( p_fname == NULL ){
    strncpy(g_download_buffer, js_code, js_code_size);
    g_download_buffer[js_code_size] = '\0';
  }else{
    long ret = load_module(p_fname, g_download_buffer, sizeof(g_download_buffer));
    if( ret != 0 )
      return ret;
  }
  response["result"] = (char*)g_download_buffer;

  return 0;
}

long endp_code_delete(JsonObject request, JsonObject response, int magic)
{
  const char *p_fname = request["fname"];

  long ret = delete_module(p_fname);
  response["result"] = ret;

  return 0;
}

long endp_code_list(JsonObject request, JsonObject response, int magic)
{
  JsonArray arry = response.createNestedArray("result");

  File dir = SPIFFS.open("/");
  if( !dir )
    return -1;
  File file = dir.openNextFile();
  int i = 0;
  while(file){
    const char *fname = file.name();
    if( strncmp(fname, MODULE_DIR, strlen(MODULE_DIR)) == 0 ){
      arry[i++] = (char*)&fname[strlen(MODULE_DIR)];
    }
    file.close();
    file = dir.openNextFile();
  }
  dir.close();

  return 0;
}


long endp_console_log(JsonObject request, JsonObject response, int magic)
{
  if( request["msg"].is<JsonArray>() ){
    JsonArray arry = request["msg"];
    int size = arry.size();
    for( int i = 0 ; i < size ; i++ ){
      const char *p_msg = arry[i];
      if( i != 0 ) Serial.print(" ");
      Serial.print(p_msg);
    }
    Serial.println("");
  }else{
    const char *p_msg = request["msg"];
    Serial.println(p_msg);
  }

  return 0;
}

EndpointEntry esp32_table[] = {
  EndpointEntry{ endp_millis, "/millis", 0 },
  EndpointEntry{ endp_reboot, "/reboot", 0 },
  EndpointEntry{ endp_restart, "/restart", 0 },
  EndpointEntry{ endp_getIpAddress, "/getIpAddress", 0 },
  EndpointEntry{ endp_getMacAddress, "/getMacAddress", 0 },
  EndpointEntry{ endp_putText, "/putText", 0 },
  EndpointEntry{ endp_code_upload, "/code-upload", 0 },
  EndpointEntry{ endp_code_download, "/code-download", 0 },
  EndpointEntry{ endp_code_delete, "/code-delete", 0 },
  EndpointEntry{ endp_code_list, "/code-list", 0 },
  EndpointEntry{ endp_console_log, "/console-log", 0 },
};
const int num_of_esp32_entry = sizeof(esp32_table) / sizeof(EndpointEntry);
