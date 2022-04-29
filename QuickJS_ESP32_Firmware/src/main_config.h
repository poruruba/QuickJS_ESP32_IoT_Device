#ifndef _MAIN_CONFIG_H_
#define _MAIN_CONFIG_H_

#include <Arduino.h>
#if defined(ARDUINO_M5Stack_ATOM)
#include <M5Atom.h>
#define _IMU_ENABLE_
#define _AUDIO_ENABLE_
#define MDNS_NAME "QuickJS_ESP32_M5Atom" // mDNSサービスホスト名
#elif defined(ARDUINO_M5Stick_C)
#include <M5StickC.h>
#define _LCD_ENABLE_
#define _RTC_ENABLE_
#define _IMU_ENABLE_
#define _AUDIO_ENABLE_
#define MDNS_NAME "QuickJS_ESP32_M5StickC" // mDNSサービスホスト名
#elif defined(ARDUINO_M5STACK_FIRE)
#define M5STACK_MPU6886
#include <M5Stack.h>
#define _LCD_ENABLE_
#define _SD_ENABLE_
#define _IMU_ENABLE_
#define _AUDIO_ENABLE_
#define MDNS_NAME "QuickJS_ESP32_M5Stack" // mDNSサービスホスト名
#elif defined(ARDUINO_M5STACK_Core2)
#include <M5Core2.h>
#define _LCD_ENABLE_
#define _RTC_ENABLE_
#define _IMU_ENABLE_
#define _SD_ENABLE_
#define _AUDIO_ENABLE_
#define MDNS_NAME "QuickJS_ESP32_M5Core2" // mDNSサービスホスト名
#endif

#define DUMMY_FNAME  "/dummy"
#define MAIN_FNAME  "/main.js"
#define MODULE_DIR  "/modules/"
#define CONFIG_FNAME  "/config.ini"
#define CONFIG_FNAME_SYSLOG  "/syslog.ini"
#define CONFIG_FNAME_MQTT  "/mqtt.ini"
#define CONFIG_FNAME_GATEWAY  "/gateway.ini"

#define CONFIG_INDEX_AUTOUPDATE 0

//#define WIFI_SSID "【固定のWiFiアクセスポイントのSSID】" // WiFiアクセスポイントのSSID
//#define WIFI_PASSWORD "【固定のWiFIアクセスポイントのパスワード】" // WiFIアクセスポイントのパスワード
#define WIFI_SSID NULL // WiFiアクセスポイントのSSID
#define WIFI_PASSWORD NULL // WiFIアクセスポイントのパスワード
#define MDNS_SERVICE "quickjs" // mDNSサービスタイプ
#define HTTP_PORT 80

#define WIFI_TIMEOUT  10000
#define SERIAL_TIMEOUT1  10000
#define SERIAL_TIMEOUT2  20000
#define SEMAPHORE_TIMEOUT   2000

#define DEFAULT_BUFFER_SIZE 4000
#define PACKET_JSON_DOCUMENT_SIZE  DEFAULT_BUFFER_SIZE
#define FILE_BUFFER_SIZE DEFAULT_BUFFER_SIZE

#define NUM_BTN_FUNC 4

#define FILE_LOADING_NONE     0
#define FILE_LOADING_RESTART  1
#define FILE_LOADING_REBOOT   2
#define FILE_LOADING_EXEC     3
#define FILE_LOADING_TEXT     4
#define FILE_LOADING_PAUSE    5
#define FILE_LOADING_START    6
#define FILE_LOADING_STOPPING 7
#define FILE_LOADING_STOP     8
extern unsigned char g_fileloading;
extern char g_download_buffer[FILE_BUFFER_SIZE];

extern bool g_autoupdate;

extern SemaphoreHandle_t binSem;

long save_jscode(const char *p_code);
long save_module(const char* p_fname, const char *p_code);
long delete_module(const char *p_fname);
long read_module(const char* p_fname, char *p_buffer, uint32_t maxlen);
long read_jscode(char *p_buffer, uint32_t maxlen);

long read_config_long(uint16_t index, long def);
long write_config_long(uint16_t index, long value);
String read_config_string(const char *fname);
long write_config_string(const char *fname, const char *text);

#endif
