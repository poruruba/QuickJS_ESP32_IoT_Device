#ifndef _MODULE_PREFS_H_
#define _MODULE_PREFS_H_

#include <Preferences.h>
#include "module_type.h"

#define PREF_MAX_BYTES_LEN  256

class Preferences2 : public Preferences{
  public:
    using Preferences::_started;
};

extern Preferences2 preferences;

extern JsModuleEntry prefs_module;

#endif
