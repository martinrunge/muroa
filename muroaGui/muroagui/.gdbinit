
python
import sys
import os
homepath = os.path.expanduser('~')
sys.path.insert(0, os.path.join(homepath,'python'))
from libstdcxx.v6.printers import register_libstdcxx_printers
register_libstdcxx_printers (None)

from qt4 import register_qt4_printers
register_qt4_printers (None)

from kde4 import register_kde4_printers
register_kde4_printers (None)

end


set print pretty on
set print object on
set print static-members on
set print vtbl on
set print demangle on
set demangle-style gnu-v3