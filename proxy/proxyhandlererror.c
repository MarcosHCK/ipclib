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
#include <config.h>
#include "proxyhandlererror.h"

G_DEFINE_QUARK (ipc-proxy-handler-error-quark, ipc_proxy_handler_error)

G_DEFINE_ENUM_TYPE (IpcProxyHandlerError, ipc_proxy_handler_error,

  G_DEFINE_ENUM_VALUE (IPC_PROXY_HANDLER_ERROR_FAILED, "failed"),
  G_DEFINE_ENUM_VALUE (IPC_PROXY_HANDLER_ERROR_UNKNOWN_CONVERSION, "unknown conversion"),
  G_DEFINE_ENUM_VALUE (IPC_PROXY_HANDLER_ERROR_UNKNOWN_METHOD, "unknown method"),
  G_DEFINE_ENUM_VALUE (IPC_PROXY_HANDLER_ERROR_UNKNOWN_PROPERTY, "unknown property"))