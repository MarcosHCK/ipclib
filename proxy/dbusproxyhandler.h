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
#ifndef __IPC_DBUS_PROXY_HANDLER__
#define __IPC_DBUS_PROXY_HANDLER__
#include <gio/gio.h>

#define IPC_TYPE_DBUS_PROXY_HANDLER (ipc_dbus_proxy_handler_get_type ())
#define IPC_DBUS_PROXY_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPC_TYPE_DBUS_PROXY_HANDLER, IpcDBusProxyHandler))
#define IPC_IS_DBUS_PROXY_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPC_TYPE_DBUS_PROXY_HANDLER))
typedef struct _IpcDBusProxyHandler IpcDBusProxyHandler;
typedef struct _IpcDBusProxyHandlerPrivate IpcDBusProxyHandlerPrivate;

#define IPC_DBUS_PROXY_HANDLER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPC_TYPE_DBUS_PROXY_HANDLER, IpcDBusProxyHandlerClass))
#define IPC_IS_DBUS_PROXY_HANDLER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPC_TYPE_DBUS_PROXY_HANDLER))
#define IPC_DBUS_PROXY_HANDLER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), IPC_TYPE_DBUS_PROXY_HANDLER, IpcDBusProxyHandlerClass))
typedef struct _IpcDBusProxyHandlerClass IpcDBusProxyHandlerClass;

#if __cplusplus
extern "C" {
#endif // __cplusplus

  struct _IpcDBusProxyHandler
    {
      GObject parent;
      IpcDBusProxyHandlerPrivate* priv;
    };

  struct _IpcDBusProxyHandlerClass
    {
      GObjectClass parent;
    };

  GType ipc_dbus_proxy_handler_get_type (void) G_GNUC_CONST;

  GDBusProxy* ipc_dbus_proxy_handler_get_proxy (IpcDBusProxyHandler* handler);
  gint ipc_dbus_proxy_handler_get_timeout (IpcDBusProxyHandler* handler);
  IpcDBusProxyHandler* ipc_dbus_proxy_handler_new (GDBusProxy* proxy);
  void ipc_dbus_proxy_handler_set_timeout (IpcDBusProxyHandler* handler, gint timeout_ms);

#if __cplusplus
}
#endif // __cplusplus

#endif // __IPC_DBUS_PROXY_HANDLER__
