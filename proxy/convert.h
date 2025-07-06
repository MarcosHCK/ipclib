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
#ifndef __IPC_PROXY_HANDLER_CONVERT__
#define __IPC_PROXY_HANDLER_CONVERT__ 1
#include <glib-object.h>
#include <proxyhandlererror.h>

#if __cplusplus
extern "C" {
#endif // __cplusplus

  G_GNUC_INTERNAL GVariant* _ipc_convert_gvalue_to_gvariant (GValue* value, GError** error);
  G_GNUC_INTERNAL void _ipc_convert_gvariant_to_gvalue (GVariant* variant, GValue* value, GError** error);

#if __cplusplus
}
#endif // __cplusplus

#endif // __IPC_PROXY_HANDLER_CONVERT__