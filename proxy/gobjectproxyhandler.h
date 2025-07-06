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
#ifndef __IPC_GOBJECT_PROXY_HANDLER__
#define __IPC_GOBJECT_PROXY_HANDLER__ 1
#include <glib-object.h>

#define IPC_TYPE_GOBJECT_PROXY_HANDLER (ipc_gobject_proxy_handler_get_type ())
#define IPC_GOBJECT_PROXY_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPC_TYPE_GOBJECT_PROXY_HANDLER, IpcGObjectProxyHandler))
#define IPC_IS_GOBJECT_PROXY_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPC_TYPE_GOBJECT_PROXY_HANDLER))
typedef struct _IpcGObjectProxyHandler IpcGObjectProxyHandler;
typedef struct _IpcGObjectProxyHandlerPrivate IpcGObjectProxyHandlerPrivate;

#define IPC_GOBJECT_PROXY_HANDLER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPC_TYPE_GOBJECT_PROXY_HANDLER, IpcGObjectProxyHandlerClass))
#define IPC_IS_GOBJECT_PROXY_HANDLER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPC_TYPE_GOBJECT_PROXY_HANDLER))
#define IPC_GOBJECT_PROXY_HANDLER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), IPC_TYPE_GOBJECT_PROXY_HANDLER, IpcGObjectProxyHandlerClass))
typedef struct _IpcGObjectProxyHandlerClass IpcGObjectProxyHandlerClass;

#if __cplusplus
extern "C" {
#endif // __cplusplus

  struct _IpcGObjectProxyHandler
    {
      GObject parent;
      IpcGObjectProxyHandlerPrivate* priv;
    };

  struct _IpcGObjectProxyHandlerClass
    {
      GObjectClass parent;
      void (*invoke) (IpcGObjectProxyHandler* handler, const gchar* method, GVariant* arguments, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data);
      GVariant* (*invoke_finish) (IpcGObjectProxyHandler* handler, GAsyncResult* res, GError** error);
    };

  GType ipc_gobject_proxy_handler_get_type (void) G_GNUC_CONST;

  GObject* ipc_gobject_proxy_handler_get_proxiee (IpcGObjectProxyHandler* self);
  void ipc_gobject_proxy_handler_invoke (IpcGObjectProxyHandler* handler, const gchar* method, GVariant* arguments, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data);
  GVariant* ipc_gobject_proxy_handler_invoke_finish (IpcGObjectProxyHandler* handler, GAsyncResult* res, GError** error);
  IpcGObjectProxyHandler* ipc_gobject_proxy_handler_new (GObject* proxiee);

#if __cplusplus
}
#endif // __cplusplus

#endif // __IPC_GOBJECT_PROXY_HANDLER__