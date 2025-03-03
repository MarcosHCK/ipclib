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

gnome = import ('gnome')

ipclib = library \
  (
    'ipclib',
    c_args: [ '-DG_LOG_DOMAIN="IpcLib"', '-DG_LOG_USE_STRUCTURED=1' ],
    include_directories: include_directories ('.'),
    dependencies: [ config_dep, libgio_dep ],
    install: true,

    sources:
      [
        'ipccall.c', 'ipccall.h',
        'ipcendpoint.c', 'ipcendpoint.h',
        'ipchandler.c', 'ipchandler.h',
        'ipclib.h',
        'ipcreplay.c', 'ipcreplay.h',

        gnome.genmarshal ('ipcmarshal', internal: true, sources: 'marshal.list',
          valist_marshallers: true)
      ])

ipclib_dep = declare_dependency \
  (
    dependencies: libgio_dep,
    include_directories: include_directories ('.'),
    link_with: ipclib,
    sources: [ 'ipccall.h', 'ipcendpoint.h', 'ipchandler.h', 'ipclib.h', 'ipcreplay.h' ]
  )

ipclib_valadep = declare_dependency \
  (
    dependencies: ipclib_dep,
    include_directories: include_directories ('.'),
    sources: files ('ipclib.vapi'),
  )

meson.override_dependency ('ipclib', ipclib_dep)
meson.override_dependency ('ipclib-vala', ipclib_valadep)