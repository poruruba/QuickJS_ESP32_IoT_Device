#ifndef _MODULE_INPUT_H_
#define _MODULE_INPUT_H_

#include "module_type.h"

#define INPUT_TOUCHPANEL  0
#define INPUT_BUTTON_A    1
#define INPUT_BUTTON_B    2
#define INPUT_BUTTON_C    3

#define FUNC_TYPE_IS_PRESSED        0
#define FUNC_TYPE_IS_RELEASED       1
#define FUNC_TYPE_WAS_PRESSED       2
#define FUNC_TYPE_WAS_RELEASED      3
#define FUNC_TYPE_PRESSED_FOR       4
#define FUNC_TYPE_RELEASED_FOR      5
#define FUNC_TYPE_WAS_RELEASE_FOR   6
#define FUNC_TYPE_IS_TOUCHED        7

extern JsModuleEntry input_module;

bool module_input_checkButtonState(uint8_t type, uint8_t value0, uint32_t value1);

#endif
