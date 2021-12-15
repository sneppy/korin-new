import re
from logging import warning
from typing import Any, Optional, Type

import gdb

_type_pattern = re.compile("^(\w[\w:]+)(<.*>)?$")
"""Pattern used to strip template arguments."""


def _decay_type(t: gdb.Type):
	"""Strips cv-qualifiers and typedefs."""

	if t.code == gdb.TYPE_CODE_REF:
		# Remove reference
		t = t.target()

	# Remove cv qualifiers and strip typedefs
	t = t.unqualified().strip_typedefs()
	return t.tag


def _get_tag_name(tag: str):
	"""Strips template arguments and return type name."""

	matches = _type_pattern.match(tag)
	if matches is None:
		raise ValueError("Invalid type tag: {}".format(tag))

	return matches[1]


class TypePrinter(object):
	"""Implements the logic to pretty-print a gdb type."""

	# The name of this subprinter
	NAME: Optional[str] = None

	def __init__(self, value: gdb.Value) -> None:
		"""Initialize with gdb value."""

		self._value = value
		self._type = value.type

	def display_hint(self) -> str:
		"""Returns a hint to tell gdb what to display."""

		raise NotImplementedError("Cannot call abstract subprinter")


class Printer(object):
	"""Class to manage gdb type printers."""

	def __init__(self, name: Optional[str]="Korin") -> None:
		"""Initialize printer."""

		self.name = name
		self._type_printers = {}

	def register(self, TypePrinterT: Type) -> None:
		"""Register a type printer."""

		assert issubclass(TypePrinterT, TypePrinter)
		lookup_name = "::".join((self.name, TypePrinterT.NAME))
		if lookup_name in self._type_printers:
			warning("Overriding existing printer for type '%s'", lookup_name)
		self._type_printers[lookup_name] = TypePrinterT

	def __call__(self, value: gdb.Value) -> Any:
		"""Create and returns the appopriate type printer."""

		# Get basic type
		t = _decay_type(value.type)
		if t is None:
			return None

		try:
			# Get tag name, then find subprinter and invoke it
			tag_name = _get_tag_name(t)
			TypePrinterT = self._type_printers[tag_name]
			return TypePrinterT(value)
		except KeyError:
			# Sub printer not found
			return None
		except Exception as e:
			# Otherwise I don't see errors
			print(type(e), e)
			raise
