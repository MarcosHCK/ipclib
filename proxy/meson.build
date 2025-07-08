# Copyright 2025-2026 MarcosHCK
# This file is part of IpcLib.
#
# IpcLib is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# IpcLib is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with IpcLib. If not, see <http://www.gnu.org/licenses/>.
#

convert = files ('convert.h', 'val2var.c', 'var2val.c')
dbusproxyhandler = files ('dbusproxyhandler.c', 'dbusproxyhandler.h')
error = files ('proxyhandlererror.c', 'proxyhandlererror.h')
gobjectproxy = files ('gobjectproxyhandler.c', 'gobjectproxyhandler.h')
proxyhandler = files ('proxyhandlercall.c', 'proxyhandlercall.h')

ipclibproxy = library \
  (
    'ipclibproxy',

    dependencies: [ config_dep, ipclib_dep, libgio_dep ],
    include_directories: include_directories ('.'),
    sources: [ convert, dbusproxyhandler, error, gobjectproxy, proxyhandler, 'proxyhandler.h' ]
  )

ipclibproxy_dep = declare_dependency \
  (
    include_directories: include_directories ('.'),
    link_with: ipclibproxy
  )

ipclibproxy_valadep = declare_dependency \
  (
    dependencies: [ ipclibproxy_dep, ipclib_valadep ],
    sources: [ 'proxyhandler.vapi' ]
  )