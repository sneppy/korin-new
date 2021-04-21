#include "core_types.h"
#include "hal/platform_crt.h"
#include "containers/containers.h"

int32 main()
{
	using namespace Containers;

	List<int32> values;
	values.pushBack(1);
	values.pushFront(2);
	values.pushFront(3);
	values.pushBack(0);
	
	values.removeAt(values.begin(), 4);

	return 0;
}
