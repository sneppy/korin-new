from typing import Any

import gdb

from gdb_pp.printer import Printer
from gdb_pp.types import ArrayPrinter, HashMapPrinter, HashSetPrinter, ListPrinter, MapPrinter, SetPrinter, StringBasePrinter, TreePrinter, TuplePrinter


def create_default_printer() -> Printer:
	"""Creates the default pretty-printer for Korin types."""

	printer = Printer()
	printer.register(TuplePrinter)
	printer.register(ArrayPrinter)
	printer.register(StringBasePrinter)
	printer.register(ListPrinter)
	printer.register(TreePrinter)
	printer.register(SetPrinter)
	#printer.register(MapPrinter) # TODO: Rewrite Map has a subclass of Tree first
	printer.register(HashSetPrinter)
	printer.register(HashMapPrinter)
	return printer


def register_default_printer(scope: Any=None) -> None:
	"""Registers the default pretty-printer.

	By default, it attempts to register the pretty-printer
	within the current objfile (if included from the source
	code). If it fails it registers it within the current
	program space. The scope can be override by passing it
	as a parameter.

	Parameter
	---------
	scope : any gdb type that can register pretty printers
		If given, register the pretty printer within this
		scope. Defaults to either the current objfile or
		the current program space.

	"""

	def_printer = create_default_printer()
	scope = scope or gdb.current_objfile() or gdb.current_progspace()
	scope.pretty_printers.append(def_printer)
