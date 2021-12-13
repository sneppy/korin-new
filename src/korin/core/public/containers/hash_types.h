#pragma once

#include "core_types.h"
#include "templates/enable_if.h"
#include "templates/types.h"
#include "templates/utility.h"

namespace Korin
{
#define HASH_BUCKET_INITIAL_COUNT (16)
#define HASH_BUCKET_LOAD_FACTOR (0.75f)

	/**
	 * @brief The hash key type is an unsigned
	 * integer, the size is platform dependant.
	 */
	using HashKey = uintp;

	namespace HashBucket_Impl
	{
		/**
		 * @brief Called to distribute more uniformly
		 * hash values, since we are using a number of
		 * buckets which is always a power of 2.
		 *
		 * Java uses a similar trick for his HashMap.
		 * See https://hg.openjdk.java.net/jdk8/jdk8/jdk/file/687fd7c7986d/src/share/classes/java/util/HashMap.java#l336
		 *
		 * @param hkey the hash key to rehash
		 * @return further hash of the hash key
		 * @{
		 */
		constexpr FORCE_INLINE uint64 hashKey(uint64 hkey)
		{
			return hkey ^ (hkey >> 32);
		}

		constexpr FORCE_INLINE uint32 hashKey(uint32 hkey)
		{
			return hkey ^ (hkey >> 16);
		}
		/** @} */

		/**
		 * @brief Computes the hash of the key, using
		 * the given hash function.
		 *
		 * The hash key may be further hashed to reduce
		 * the number of collisions.
		 *
		 * @tparam HashT the type of the hash function
		 * @param key the key object to hash
		 * @param h the hash function to use
		 * @return an hash key for the given key
		 */
		template<typename HashT>
		constexpr FORCE_INLINE HashKey computeHash(auto const& key, HashT&& h)
		{
			HashKey hkey = h(key);
#if KORIN_RELEASE
			hkey = hashKey(hkey);
#endif
			return hkey;
		}
	} // namespace HashBucket_Impl

	/**
	 * @brief A bucket used by hash array-like
	 * containers.
	 *
	 * It is implemented as a forward linked list.
	 *
	 * Each element of the bucket stores the value
	 * and the hash key of that value. Note that
	 * if the value is externally modified the
	 * hash key could be invalidated.
	 *
	 * @tparam T the type of the value stored
	 * @tparam HashPolicyT the policy used to hash
	 * the value
	 */
	template<typename T, typename HashPolicyT>
	struct HashBucket
	{
		/* The value stored in this position. */
		T value;

		/* The hash key of the value stored in this position. */
		HashKey hkey;

		/**
		 * @brief Construct a new bucket node with the
		 * given value.
		 */
		HashBucket(auto&& inValue, HashKey inHkey = {})
			: value{FORWARD(inValue)}
			, hkey{inHkey}
		{
			//
		}

		HashBucket(T const& inValue)
			: HashBucket{inValue, {}/* , HashBucket_Impl::computeHash(inValue, HashPolicyT{}) */}
		{
			//
		}

		HashBucket(T&& inValue)
			: HashBucket{move(inValue), {}/* , HashBucket_Impl::computeHash(inValue, HashPolicyT{}) */}
		{
			//
		}
	};

	/**
	 * @brief The default hash policy.
	 *
	 * Defines an hash function for all POD
	 * types and uses a user-defined function
	 * for POD types.
	 */
	struct DefaultHashPolicy
	{
		/**
		 * @brief Computes the hash key for the given
		 * key object. If the type is non-POD, this
		 * policy assumes it defines a class method
		 * with the name @c toHashKey which returns
		 * a valid hash key for the object.
		 *
		 * @tparam KeyT the type of the key
		 * @param key the key object to hash
		 * @return hash for the key object
		 * @{
		 */
		template<typename KeyT>
		constexpr FORCE_INLINE typename EnableIf<!IsPOD<KeyT>::value, HashKey>::Type operator()(KeyT const& key) const
		{
			// Use class method for non-POD type
			return static_cast<HashKey>(key.toHashKey());
		}

		template<typename IntT>
		constexpr FORCE_INLINE typename EnableIf<IsIntegral<IntT>::value, HashKey>::Type operator()(IntT key) const
		{
			// Identity hash
			return static_cast<HashKey>(key);
		}

		constexpr FORCE_INLINE HashKey operator()(double key) const
		{
			constexpr uint64 mask = ~0xfull;
			return *reinterpret_cast<HashKey*>(&key) & mask;
		}

		constexpr FORCE_INLINE HashKey operator()(float key) const
		{
			return (*this)(static_cast<double>(key));
		}
		/** @} */
	};

	/**
	 * @brief A general-purpose hash function, suitable
	 * for non-cryptographic uses, such as generating
	 * the hash key for hash tables.
	 *
	 * https://sites.google.com/site/murmurhash/
	 *
	 * @param key data of the key to hash
	 * @param len lenght of the data in Bytes
	 * @param seed optional random seed
	 * @return the hash of key
	 */
	HashKey murmur(void const* key, sizet len, HashKey seed = -1);
} // namespace Korin
