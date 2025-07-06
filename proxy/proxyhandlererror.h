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
#ifndef __IPC_PROXY_HANDLER_ERROR__
#define __IPC_PROXY_HANDLER_ERROR__ 1
#include <glib-object.h>

typedef enum
{
  IPC_PROXY_HANDLER_ERROR_FAILED,
  IPC_PROXY_HANDLER_ERROR_UNKNOWN_CONVERSION,
  IPC_PROXY_HANDLER_ERROR_UNKNOWN_METHOD,
  IPC_PROXY_HANDLER_ERROR_UNKNOWN_PROPERTY,
} IpcProxyHandlerError;

#define IPC_PROXY_HANDLER_ERROR (ipc_proxy_handler_error_quark ())

#if __cplusplus
extern "C" {
#endif // __cplusplus

  GType ipc_proxy_handler_error_get_type (void) G_GNUC_CONST;
  GQuark ipc_proxy_handler_error_quark (void) G_GNUC_CONST;

#if __cplusplus
}
#endif // __cplusplus

#endif // __IPC_PROXY_HANDLER_ERROR__