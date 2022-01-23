from typing import Any, Iterable, Iterator, Tuple

import gdb

from .printer import TypePrinter


class TuplePrinter(TypePrinter):
	"""Implements a pretty-printer for tuples."""

	NAME = "Tuple"

	def __init__(self, value: gdb.Value) -> None:

		super().__init__(value)

		self.num_items = value["numItems"]

	def display_hint(self) -> str:

		return "array"

	def to_string(self) -> str:

		return "Tuple<>[%d]" % (self.num_items)

	def children(self) -> Iterator[Tuple[str, Any]]:

		base_type = self._type.fields()[0]
		base = self._value[base_type]

		for idx in range(self.num_items):
			yield ("[%d]" % idx, base["item"])
			# Next base
			base_type = base.type.fields()[0]
			base = base[base_type]


class ArrayBasePrinter(TypePrinter):
	"""Implements a pretty-printer for array base type. """

	NAME = "ArrayBase"

	def __init__(self, value: gdb.Value) -> None:

		super().__init__(value)

		self.buffer = value["buffer"]
		self.size = value["size"]
		self.num_items = value["numItems"]

	def display_hint(self) -> str:

		return "array"

	def to_string(self) -> str:

		return "ArrayBase<%s>[%d]".format(self._type.template_arguments(0), self.num_items)

	def children(self) -> Iterator[Tuple[str, Any]]:

		for idx in range(self.num_items):
			item = (self.buffer + idx).dereference()
			yield ("[%d]" % idx, item)


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

		if self.array["buffer"] == 0:
			return ""
		else:
			return self.array["buffer"].string()


class ListPrinter(TypePrinter):
	"""Implements a pretty-printer for linked-lists."""

	NAME = "Tree"


	def __init__(self, value: gdb.Value) -> None:

		super().__init__(value)

		self.head = value["head"]
		self.tail = value["tail"]
		self.num_nodes = value["numNodes"]

	def display_hint(self) -> str:

		return "array"

	def to_string(self) -> str:

		return "List<%s>[%d]" % (self._type.template_arguments(0), self.num_nodes)

	def children(self) -> Iterator[Tuple[str, Any]]:

		it = self.head
		idx = 0
		while it:
			node = it.dereference()
			yield ("[%d]" % idx, node["value"])
			it = node["next"]
			idx += 1


class TreePrinter(TypePrinter):
	"""Implements a pretty-printer for Tree-based types.

	Tree nodes are printed as a linked list rather than an
	actual binary tree.

	"""

	NAME = "Tree"

	def __init__(self, value: gdb.Value) -> None:

		super().__init__(value)

		self.root = value["root"]
		self.num_nodes = value["numNodes"]

	def display_hint(self) -> str:

		return "array"

	def to_string(self) -> str:

		return "Tree<%s>[%d]" % (self._type.template_arguments(0), self.num_nodes)

	def children(self) -> Iterator[Tuple[str, Any]]:

		def _get_leftmost(root):

			it = root
			while root:
				it = root
				node = root.dereference()
				root = node["left"]
			return it

		# Iterate as a list
		it = _get_leftmost(self.root)
		idx = 0
		while it:
			node = it.dereference()
			yield ("[%d]" % idx, node["value"])
			it = node["next"]
			idx += 1


class SetPrinter(TreePrinter):
	"""Implements a pretty-printer for sets."""

	NAME = "Set"

	def to_string(self) -> str:

		return "Set<%s>[%d]" % (self._type.template_arguments(0), self.num_nodes)


class MapPrinter(TreePrinter):
	"""Implements a pretty-printer for maps."""

	NAME = "Map"

	def display_hint(self) -> str:
		"""  """

		return "map"

	def to_string(self) -> str:

		return "Map<%s, %s>[%d]" % (self._type.template_arguments(0), self._type.template_arguments(1), self.num_nodes)

	def children(self) -> Iterator[Tuple[str, Any]]:

		idx = 0
		for _, item in super().children():
			yield ("[%d]" % idx, item["first"])
			yield ("[%d]" % (idx + 1), item["second"])
			idx += 2


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
