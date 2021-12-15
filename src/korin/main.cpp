#include "core_types.h"
#include "hal/platform_crt.h"
#include "containers/containers.h"

LOAD_DEBUG_SCRIPT("scripts/gdb-pp/register.py", GDB_SCRIPT_PYTHON_FILE)

int32 main()
{
	using namespace Korin;

	return 0;
}
