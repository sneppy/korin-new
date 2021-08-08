#pragma once

#include "core_types.h"
#include "templates/ordering.h"

namespace Containers
{
	template<typename> struct QueueNode;
	template<typename> struct ListNode;
	template<typename> struct BinaryNode;
	template<typename> struct Pair;
	template<typename> struct Tuple;

	template<typename>							class Stack;
	template<typename>							class Queue;
	template<typename>							class List;
	template<typename>							class Array;
	template<typename, typename = GreaterThan>	class Tree;
	template<typename, typename, typename>		class Map;
	template<typename, typename = GreaterThan>	class Set;
	template<typename>							class HashMap;
	template<typename>							class HashSet;
} // namespace Containers
