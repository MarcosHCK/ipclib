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

tests = \
  [
    { 'name': 'core', 'deps': [ ipclib_valadep ],
      'sources': [ 'core.vala' ] },
  ]

valac = meson.get_compiler ('vala')

foreach test_ : tests

  deps = test_.get ('deps', [ ])
  libs = test_.get ('libs', [ ])
  name = test_.get ('name')
  sources = test_.get ('sources')
  timeout = test_.get ('timeout', 30)

  assert (sources.length () > 0, 'Specify at least a test source file')

  test \
    (
      name,

      executable \
        ( '@0@-bin'.format (name),
          dependencies: [ deps, libgio_dep, valac.find_library ('gio-2.0') ],
          link_with: libs,
          sources: [ 'base.vala', sources ] ),

      env: [ 'G_TEST_SRCDIR=@0@'.format (meson.current_source_dir () / '..'),
             'G_TEST_BUILDDIR=@0@'.format (meson.current_build_dir () / '..') ],

      timeout: timeout,
    )
endforeach