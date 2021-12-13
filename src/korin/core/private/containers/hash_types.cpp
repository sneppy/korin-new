#include "containers/hash_types.h"

namespace Korin
{
	namespace
	{
		/**
		 * @brief 64-bit key implementation of the
		 * general-purpose murmur hash.
		 *
		 * @param key key data to hash
		 * @param len length of the key data in Bytes
		 * @param seed random seed
		 * @return Hash key
		 */
		FORCE_INLINE HashKey murmur64(void const* key, sizet len, HashKey seed)
		{
			uint64 const m = 0xc6a4a7935bd1e995;
			uint64 h = seed ^ (len * m);
			uint64 const* const begin = reinterpret_cast<uint64 const*>(key);
			uint64 const* const end = begin + (len / 8);
			uint64 const* it = begin;

			for (; it != end; ++it)
			{
				uint64 k = *it;

				k *= m;
				k ^= k >> 47;
				k *= m;

				h ^= k;
				h *= m;
			}

			// Handle remaining bytes
			uint8 const* jt = reinterpret_cast<uint8 const*>(it);
			switch (len & 0x7)
			{
			case 7: h ^= static_cast<uint64>(jt[6]) << 48;
			case 6: h ^= static_cast<uint64>(jt[5]) << 40;
			case 5: h ^= static_cast<uint64>(jt[4]) << 32;
			case 4: h ^= static_cast<uint64>(jt[3]) << 24;
			case 3: h ^= static_cast<uint64>(jt[2]) << 16;
			case 2: h ^= static_cast<uint64>(jt[1]) << 8;
			case 1: h ^= static_cast<uint64>(jt[0]);
			        h *= m;
			}

			h ^= h >> 47;
			h *= m;
			h ^= h >> 47;

			return static_cast<HashKey>(h);
		}
	} // namespace

	HashKey murmur(void const* key, sizet len, HashKey seed)
	{
		return murmur64(key, len, seed);
	}
} // namespace Korin
