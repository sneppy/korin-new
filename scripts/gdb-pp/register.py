import os
import sys

# Add this folder to PYTHONPATH
sys.path.insert(0, os.path.dirname(__file__))


from gdb_pp import register_default_printer

# Register the default pp
register_default_printer()
