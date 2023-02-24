#ifndef _MODULE_LCD_H_
#define _MODULE_LCD_H_

#include "module_type.h"

#ifdef _LCD_ENABLE_

#define LGFX_USE_V1
#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>
extern LGFX lcd;

extern JsModuleEntry lcd_module;

#define FUNC_TYPE_WIDTH 1
#define FUNC_TYPE_HEIGHT 2
#define FUNC_TYPE_DEPTH 3
#define FUNC_TYPE_FONTHEIGHT 4

#define FUNC_TYPE_DRAWRECT  0
#define FUNC_TYPE_FILLRECT  1
#define FUNC_TYPE_DRAWROUNDRECT  2
#define FUNC_TYPE_FILLROUNDRECT  3
#define FUNC_TYPE_DRAWCIRCLE  4
#define FUNC_TYPE_FILLCIRCLE  5

long module_lcd_setFont(uint16_t size);

#endif

#endif
