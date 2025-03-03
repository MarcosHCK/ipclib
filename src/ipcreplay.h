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
#ifndef __IPC_REPLAY__
#define __IPC_REPLAY__ 1
#include <glib.h>

#if __cplusplus
extern "C" {
#endif // __cplusplus

  GVariant* ipc_reply_pack (GVariant* reply, GError* error);
  GVariant* ipc_reply_unpack (GVariant* reply, GError** error);

#if __cplusplus
}
#endif // __cplusplus

#endif // __IPC_REPLAY__
