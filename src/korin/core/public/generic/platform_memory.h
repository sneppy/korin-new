#pragma once

/**
 * @brief Memory abstraction layer.
 */
struct GenericPlatformMemory
{
	/**
	 * @brief Copy memory between non-overlapping
	 * buffers.
	 * 
	 * @param dst ptr to destination buffer
	 * @param src ptr to source buffer
	 * @param size number of Bytes to copy
	 */
	static FORCE_INLINE void memcpy(void* RESTRICT dst, void const* RESTRICT src, sizet size)
	{
		::memcpy(dst, src, size);
	}

	/**
	 * @brief Copy source buffer into destination
	 * buffer. The buffers may overlap.
	 * 
	 * @param dst ptr to destination buffer
	 * @param src ptr to source buffer
	 * @param size number of Bytes to copy
	 */
	static FORCE_INLINE void memmove(void* dst, void const* src, sizet size)
	{
		::memmove(dst, src, size);
	}
};
