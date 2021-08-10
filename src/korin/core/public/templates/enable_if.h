#pragma once

#include "core_types.h"

/**
 * @brief If condition is true exposes
 * Type. Can be used to SFINAE.
 * 
 * @tparam cond boolean condition
 * @tparam T type to expose
 */
template<bool cond, typename T = void>
struct EnableIf
{
	//
};

template<typename T> struct EnableIf<true, T> { using Type = T; };
