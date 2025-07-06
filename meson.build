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

project \
  (
    'ipclib', 'c',

    default_options : [ 'c_std=c17', 'warning_level=3' ],
    license : 'GPL-3.0-or-later',
    license_files : [ 'COPYING' ],
    meson_version : '>= 1.3.2',

    version : run_command ('version.py', 'get-version', check : true).stdout ().strip (),
  )

cc = meson.get_compiler ('c')

meson.add_dist_script ('version.py', 'set-dist', meson.project_version ())

git_version = meson.project_version ()
git_version_list = git_version.split ('-')
git_version_list2 = git_version_list.get (0).split ('.')
git_version_major = git_version_list2.get (0)
git_version_minor = git_version_list2.get (1)
git_version_micro = git_version_list2.get (2)
git_version_stage = git_version_list.get (1)

config = configuration_data ()

config.set ('DEBUG', get_option ('debug').to_int ())
config.set_quoted ('PACKAGE_BUGREPORT', 'nouser@nohost.com')
config.set_quoted ('PACKAGE_NAME', meson.project_name ())
config.set_quoted ('PACKAGE_STRING', '@0@ @1@'.format (meson.project_name (), meson.project_version ()))
config.set_quoted ('PACKAGE_TARNAME', meson.project_name ())
config.set_quoted ('PACKAGE_URL', 'https://github.com/MarcosHCK/ipclib')
config.set_quoted ('PACKAGE_VERSION', git_version)
config.set ('PACKAGE_VERSION_MAJOR', git_version_major)
config.set ('PACKAGE_VERSION_MINOR', git_version_minor)
config.set ('PACKAGE_VERSION_MICRO', git_version_micro)
config.set_quoted ('PACKAGE_VERSION_STAGE', git_version_stage)

config_dep = declare_dependency \
  (
    include_directories: include_directories ('.'),
    sources: configure_file (output: 'config.h', configuration: config),
  )

libgio_dep = dependency ('gio-2.0', required: true)

subdir ('src')
subdir ('proxy')

########################
# Summaries            #
########################

summary ({
    'C compiler' : cc.get_id (),
    'debug build' : get_option ('debug') ? 'yes' : 'no',
    'host cpu' : host_machine.cpu_family (),
    'host endian' : host_machine.endian (),
    'host system' : host_machine.system (),
  }, section: 'Build environment')

if build_machine.system () != host_machine.system ()

  summary ({
      'build cpu' : build_machine.cpu_family (),
      'build endian' : build_machine.endian (),
      'build system' : build_machine.system (),
    }, section: 'Build environment')
endif
