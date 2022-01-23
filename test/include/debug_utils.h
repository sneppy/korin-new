#pragma once

#include "hal/platform.h"


#ifdef __GNUC__
// Load gdb pretty printers
LOAD_DEBUG_SCRIPT("../../../scripts/gdb-pp/register_dev.py", GDB_SCRIPT_PYTHON_FILE)
#endif
