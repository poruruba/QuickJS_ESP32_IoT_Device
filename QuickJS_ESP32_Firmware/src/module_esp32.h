#ifndef _MODULE_ESP32_H_
#define _MODULE_ESP32_H_

#include "module_type.h"

extern JsModuleEntry esp32_module;
extern JsModuleEntry console_module;

long esp32_initialize(void);
void esp32_update(void);

long syslog_send(const char *p_message);
long syslog_changeServer(const char *host, uint16_t port);

#endif
