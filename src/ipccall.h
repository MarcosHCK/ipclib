/* Copyright 2025-2026 MarcosHCK
 * This file is part of IpcLib.
 *
 * IpcLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IpcLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IpcLib. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __IPC_CALL__
#define __IPC_CALL__ 1
#include <glib.h>

#define IPC_CALL_SIGNATURE "(sr)"

#if __cplusplus
extern "C" {
#endif // __cplusplus

  GVariant* ipc_call_pack (const gchar* name, GVariant* arguments);
  gchar* ipc_call_unpack (GVariant* call, GVariant** arguments);

#if __cplusplus
}
#endif // __cplusplus

#endif // __IPC_CALL__
