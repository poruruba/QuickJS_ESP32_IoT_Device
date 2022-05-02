#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <SPIFFS.h>

#include "quickjs_esp32.h"
#include "main_config.h"

#include "endpoint_types.h"
#include "endpoint_packet.h"

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

extern const char jscode_default[] asm("_binary_rom_default_js_start");
extern const char jscode_epilogue[] asm("_binary_rom_epilogue_js_start");

char *js_modules_code = NULL;
bool g_autoupdate = false;

char g_download_buffer[FILE_BUFFER_SIZE];
unsigned char g_fileloading = FILE_LOADING_NONE;

ESP32QuickJS qjs;
AsyncWebServer server(HTTP_PORT);
SemaphoreHandle_t binSem;

static long m5_initialize(void);
static long start_qjs(void);
static char* load_jscode(void);
static long load_all_modules(void);

void notFound(AsyncWebServerRequest *request)
{
  if (request->method() == HTTP_OPTIONS){
    request->send(200);
  }else{
    request->send(404);
  }
}

void setup()
{
  SPIFFS.begin();
  delay(100);

  if( !SPIFFS.exists(DUMMY_FNAME) ){
    File fp = SPIFFS.open(DUMMY_FNAME, FILE_WRITE);
    if( !fp ){
      Serial.println("SPIFFS FORMAT START");
      SPIFFS.format();
      Serial.println("SPIFFS FORMAT END");

      File fp = SPIFFS.open(DUMMY_FNAME, FILE_WRITE);
      fp.close();
    }else{
      fp.close();
    }
  }
  if( !SPIFFS.exists(MODULE_DIR) ){
      SPIFFS.mkdir(MODULE_DIR);
  }

  binSem = xSemaphoreCreateBinary();
  xSemaphoreGive(binSem);

  long ret;
  ret = m5_initialize();
  if( ret == 0 ){
    packet_initialize();
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

    if( is_wifi_connected() ){
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
//      server.serveStatic("/", SPIFFS, "/html/").setDefaultFile("index.html");
      server.onNotFound(notFound);
      server.begin();
    }
  }

  long conf = read_config_long(CONFIG_INDEX_AUTOUPDATE, 0);
  g_autoupdate = (conf != 0) ? true : false;
  if( g_autoupdate )
    Serial.println("autoupdate: on");

  qjs.initialize_modules();

  start_qjs();
}

void loop()
{
  if( g_fileloading == FILE_LOADING_PAUSE || g_fileloading == FILE_LOADING_STOP ){
    delay(100);
    return;
  }else{
// FILE_LOADING_NONE
// FILE_LOADING_RESTART
// FILE_LOADING_REBOOT
// FILE_LOADING_EXEC
// FILE_LOADING_TEXT
// FILE_LOADING_START
// FILE_LOADING_STOPPING
  }

  if( g_autoupdate )
    qjs.update_modules();

  // For timer, async, etc.
  if( !qjs.loop() ){
    qjs.end();

    if( g_fileloading != FILE_LOADING_NONE ){
      if( g_fileloading == FILE_LOADING_REBOOT ){
        Serial.println("[now rebooting]");
        delay(2000);
        ESP.restart();
        return;
      }else if( g_fileloading == FILE_LOADING_RESTART ){
        Serial.println("[now restarting]");
        delay(2000);
      }else if( g_fileloading == FILE_LOADING_STOPPING ){
        Serial.println("[now stopping]");
        delay(2000);
      }else if( g_fileloading == FILE_LOADING_START ){
        Serial.println("[now starting]");
      }
    }

    if( g_fileloading == FILE_LOADING_STOPPING ){
      g_fileloading = FILE_LOADING_STOP;
    }else{
      start_qjs();
    }
  }else{
    if( g_fileloading == FILE_LOADING_EXEC ){
      qjs.exec(g_download_buffer);
    }
  }

  delay(1);
}

static long start_qjs(void)
{
    qjs.begin();

  long ret1 = load_all_modules();
  if( ret1 != 0 ){
      Serial.println("[can't load module]");
    }

  char *js_code = load_jscode();
  if( js_code != NULL ){
    Serial.println("[executing]");
    qjs.exec(js_code);
    free(js_code);
    js_code = NULL;
  }else{
    Serial.println("[can't load main]");
    qjs.exec(jscode_default);
  }

  return (js_code != NULL) ? ret1 : -1;
}

long save_jscode(const char *p_code)
{
  File fp = SPIFFS.open(MAIN_FNAME, FILE_WRITE);
  if( !fp )
    return -1;
  fp.write((uint8_t*)p_code, strlen(p_code) + 1);
  fp.close();

  return 0;
}

long read_jscode(char *p_buffer, uint32_t maxlen)
{
  if( !SPIFFS.exists(MAIN_FNAME) ){
    p_buffer = '\0';
    return 0;
  }
  File fp = SPIFFS.open(MAIN_FNAME, FILE_READ);
  if( !fp )
    return -1;
  size_t size = fp.size();
  if( size + 1 > maxlen ){
    fp.close();
    return -1;
  }
  fp.readBytes(p_buffer, size);
  fp.close();
  p_buffer[size] = '\0';

  return 0;
  }
    
static char* load_jscode(void)
{
    if( !SPIFFS.exists(MAIN_FNAME) )
    return NULL;
    File fp = SPIFFS.open(MAIN_FNAME, FILE_READ);
    if( !fp )
    return NULL;
    size_t size = fp.size();
  char* js_code = (char*)malloc(size + strlen(jscode_epilogue) + 1);
    if( js_code == NULL ){
      fp.close();
    return NULL;
    }
    fp.readBytes(js_code, size);
    fp.close();
    js_code[size] = '\0';

    strcat(js_code, jscode_epilogue);

  return js_code;
}

long save_module(const char* p_fname, const char *p_code)
{
  char filename[64];
  if( strlen(p_fname) > sizeof(filename) - strlen(MODULE_DIR) - 1)
    return -1;
  strcpy(filename, MODULE_DIR);
  strcat(filename, p_fname);

  File fp = SPIFFS.open(filename, FILE_WRITE);
  if( !fp )
    return -1;
  fp.write((uint8_t*)p_code, strlen(p_code));
  fp.close();

  return 0;
}

long read_module(const char* p_fname, char *p_buffer, uint32_t maxlen)
{
  char filename[64];
  if( strlen(p_fname) > sizeof(filename) - strlen(MODULE_DIR) - 1)
    return -1;
  strcpy(filename, MODULE_DIR);
  strcat(filename, p_fname);

  File fp = SPIFFS.open(filename, FILE_READ);
  if( !fp )
    return -1;
  uint32_t size = fp.size();
  if( maxlen < size + 1 ){
    fp.close();
    return -1;
  }
  fp.read((uint8_t*)p_buffer, size);
  p_buffer[size] = '\0';
  fp.close();

  return 0;
}

long delete_module(const char *p_fname)
{
  char filename[64];
  if( strlen(p_fname) > sizeof(filename) - strlen(MODULE_DIR) - 1)
    return -1;
  strcpy(filename, MODULE_DIR);
  strcat(filename, p_fname);

  bool ret = SPIFFS.remove(filename);
  return ret ? 0 : -1;
}

static long get_all_modules_size(void)
{
  int32_t sum = 0;

  File dir = SPIFFS.open("/");
  if( !dir )
    return -1;
  File file = dir.openNextFile();
  while(file){
    const char *fname = file.name();
    if( strncmp(fname, MODULE_DIR, strlen(MODULE_DIR)) == 0 )
      sum += file.size();
    file.close();
    file = dir.openNextFile();
  }
  dir.close();

  return sum;
}

static long load_all_modules(void)
{
  if( js_modules_code != NULL ){
    free(js_modules_code);
    js_modules_code = NULL;
  }

  long all_size = get_all_modules_size();
  if( all_size < 0 )
    return -1;

  File dir = SPIFFS.open("/");
  if( !dir )
    return -1;

  js_modules_code = (char*)malloc(all_size + 1);
  js_modules_code[0] = '\0';
  int32_t js_modules_len = 0;

  File file = dir.openNextFile();
  char module_name[32];
  while(file){
    const char *fname = file.name();
    if( strncmp(fname, MODULE_DIR, strlen(MODULE_DIR)) == 0 &&
        strlen(&fname[strlen(MODULE_DIR)]) <= sizeof(module_name) - 1)
    {
      strcpy(module_name, &fname[strlen(MODULE_DIR)]);
      size_t size = file.size();
      file.readBytes(&js_modules_code[js_modules_len], size);
      file.close();
      js_modules_code[js_modules_len + size] = '\0';

      long ret = qjs.load_module(&js_modules_code[js_modules_len], size, module_name);
      if( ret != 0 ){
        dir.close();
        Serial.printf("load module(%s) failed\n", module_name);
        return -1;
      }
      Serial.printf("load module(%s) loaded\n", module_name);
      js_modules_len += size + 1;
    }else{
      file.close();
    }
    file = dir.openNextFile();
  }
  dir.close();

  return 0;
}

static long wifi_connect(const char *ssid, const char *password, unsigned long timeout)
{
  Serial.println("");
  Serial.print("WiFi Connenting");

  if( ssid == NULL && password == NULL )
    WiFi.begin();
  else
    WiFi.begin(ssid, password);
  unsigned long past = 0;
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
    past += 500;
    if( past > timeout ){
      Serial.println("\nCan't Connect");
      return -1;
    }
  }
  Serial.print("\nConnected : IP=");
  Serial.print(WiFi.localIP());
  Serial.print(" Mac=");
  Serial.println(WiFi.macAddress());

  return 0;
}

static long m5_initialize(void)
{
#if defined(ARDUINO_M5Stick_C)
  M5.begin(true, true, true);
#elif defined(ARDUINO_M5Stack_ATOM)
  M5.begin(true, true, false);
#elif defined(ARDUINO_M5STACK_FIRE)
  M5.begin(true, true, true, true);
#elif defined(ARDUINO_M5STACK_Core2)
  M5.begin(true, true, true, true);
  M5.Axp.SetSpkEnable(true);
#endif
//  Serial.begin(115200);
  Serial.println("[initializing]");

  long ret = wifi_connect(WIFI_SSID, WIFI_PASSWORD, WIFI_TIMEOUT);
  if( ret != 0 ){
    char ssid[32 + 1] = {'\0'};
    Serial.print("\ninput SSID:");
    Serial.setTimeout(SERIAL_TIMEOUT1);
    ret = Serial.readBytesUntil('\r', ssid, sizeof(ssid) - 1);
    if( ret <= 0 )
      return -1;

    delay(10);
    Serial.read();
    char password[32 + 1] = {'\0'};
    Serial.print("\ninput PASSWORD:");
    Serial.setTimeout(SERIAL_TIMEOUT2);
    ret = Serial.readBytesUntil('\r', password, sizeof(password) - 1);
    if( ret <= 0 )
      return -1;

    delay(10);
    Serial.read();
    Serial.printf("\nSSID=%s PASSWORD=", ssid);
    for( int i = 0 ; i < strlen(password); i++ )
      Serial.print("*");
    Serial.println("");
    ret = wifi_connect(ssid, password, WIFI_TIMEOUT);
    if( ret != 0 )
      return ret;
  }

  return 0;
}

long read_config_long(uint16_t index, long def)
{
  File fp = SPIFFS.open(CONFIG_FNAME, FILE_READ);
  if( !fp )
    return def;
  
  size_t fsize = fp.size();
  if( fsize < (index + 1) * sizeof(long) )
    return def;

  bool ret = fp.seek(index * sizeof(long));
  if( !ret ){
    fp.close();
    return def;
  }

  long value;
  if( fp.read((uint8_t*)&value, sizeof(long)) != sizeof(long) ){
    fp.close();
    return def;
  }
  fp.close();

  return value;
}

long write_config_long(uint16_t index, long value)
{
  File fp = SPIFFS.open(CONFIG_FNAME, FILE_WRITE);
  if( !fp )
    return -1;
  
  size_t fsize = fp.size();
  long temp = 0;
  if( fsize < index * sizeof(long) ){
    fp.seek(fsize / sizeof(long) * sizeof(long));
    for( int i = fsize / sizeof(long) ; i < index ; i++ )
      fp.write((uint8_t*)&temp, sizeof(long));
  }


  if( fp.write((uint8_t*)&value, sizeof(long)) != sizeof(long) ){
    fp.close();
    return -1;
  }
  fp.close();

  return 0;
}

String read_config_string(const char *fname)
{
  File fp = SPIFFS.open(fname, FILE_READ);
  if( !fp )
    return String("");

  String text = fp.readString();
  fp.close();

  return text;
}

long write_config_string(const char *fname, const char *text)
{
  File fp = SPIFFS.open(fname, FILE_WRITE);
  if( !fp )
    return -1;

  long ret = fp.write((uint8_t*)text, strlen(text));
  fp.close();
  if( ret != strlen(text) )
    return -1;

  return 0;
}
