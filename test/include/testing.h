#pragma once

#include "core_types.h"
#include "hal/platform_crt.h"

namespace Korin::Testing
{
	struct Object
	{
		Object(sizet inSize)
			: size{inSize}
		{
			mem = ::malloc(size);
		}

		Object()
			: Object(100)
		{
			//
		}

		Object(Object const& other)
			: size{other.size}
		{
			mem = ::malloc(size);
			::memcpy(mem, other.mem, size);
		}

		Object(Object&& other)
			: mem{other.mem}
			, size{other.size}
		{
			other.mem = nullptr;
			other.size = 0;
		}

		Object& operator=(Object const& other)
		{
			if (other.size != size)
			{
				mem = ::realloc(mem, other.size);
				size = other.size;
			}
			
			::memcpy(mem, other.mem, size);

			return *this;
		}

		Object& operator=(Object&& other)
		{
			::free(mem);
			mem = other.mem;
			size = other.size;
			other.mem = nullptr;
			other.size;
			return *this;
		}

		~Object()
		{
			if (mem)
			{
				::free(mem);
			}

			size = 0;
		}

		sizet getSize() const
		{
			return size;
		}

		void test()
		{
			char* it = reinterpret_cast<char*>(mem);
			for (int32 i = 0; i < size; ++i)
			{
				*it = i & 0xff;
			}
		}

	protected:
		void* mem;
		sizet size;
	};
} // namespace Korin::Testing
