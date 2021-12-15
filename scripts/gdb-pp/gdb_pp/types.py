from typing import Any, Iterator, Tuple

import gdb

from .printer import TypePrinter


class ArrayPrinter(TypePrinter):
	"""Implements a pretty-printer for arrays."""

	NAME = "Array"

	def __init__(self, value: gdb.Value) -> None:

		super().__init__(value)

		self.data = value["data"]
		self.size = value["size"]
		self.count = value["count"]

	def display_hint(self) -> str:

		return "array"

	def to_string(self) -> str:

		return "Array<%s>[%d]" % (self._type.template_argument(0), self.count)

	def children(self) -> Iterator[Tuple[str, Any]]:

		it = self.data
		for idx in range(self.count):
			# Yield all the items in the array
			item = it.dereference()
			yield ("[%d]" % idx, item)
			it += 1


class StringBasePrinter(TypePrinter):
	"""Implements a pretty-printer for strings."""

	NAME = "StringBase"

	def __init__(self, value: gdb.Value) -> None:

		super().__init__(value)

		self.array = value["array"]

	def display_hint(self) -> str:

		return "string"

	def to_string(self) -> None:

		if self.array["data"] == 0:
			return ""
		else:
			return self.array["data"].string()


class HashTablePrinter(TypePrinter):
	"""Base class to pretty-print HashTable-based types."""

	NAME = "HashTable"

	def __init__(self, value: gdb.Value) -> None:

		super().__init__(value)

		self.buckets = value["buckets"]
		self.num_buckets = value["numBuckets"]
		self.num_items = value["numItems"]
		self.before_head = value["placeholder"]

	def display_hint(self) -> str:

		return "array"

	def children(self) -> Iterator[Any]:

		idx = 0
		it = self.before_head["next"]
		while it:
			node = it.dereference()
			yield ("[%d]" % idx, node["value"])
			it = node["next"]


class HashMapPrinter(HashTablePrinter):
	"""Implements a pretty-printer for hash maps."""

	NAME = "HashMap"

	def display_hint(self) -> str:

		return "map"

	def to_string(self) -> str:

		return "HashMap<%s, %s>[%d]" % (self._type.template_argument(0), self._type.template_argument(1), self.num_items)

	def children(self) -> Iterator[Any]:

		idx = 0
		for _, item in super().children():
			# Yield key-value pairs
			yield ("[%d]" % idx, item["first"])
			yield ("[%d]" % (idx + 1), item["second"])
			idx += 2


class HashSetPrinter(HashTablePrinter):
	"""Implements a pretty-printer for hash sets."""

	NAME = "HashSet"

	def to_string(self) -> str:

		return "HashSet<%s>[%d]" % (self._type.template_argument(0), self.num_items)
