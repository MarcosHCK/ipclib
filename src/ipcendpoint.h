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
#ifndef __IPC_ENDPOINT__
#define __IPC_ENDPOINT__ 1
#include <gio/gio.h>
#include <ipchandler.h>

#define IPC_TYPE_ENDPOINT (ipc_endpoint_get_type ())
#define IPC_ENDPOINT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPC_TYPE_ENDPOINT, IpcEndpoint))
#define IPC_IS_ENDPOINT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPC_TYPE_ENDPOINT))
typedef struct _IpcEndpoint IpcEndpoint;

#define IPC_ENDPOINT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), IPC_TYPE_ENDPOINT, IpcEndpointClass))
#define IPC_IS_ENDPOINT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), IPC_TYPE_ENDPOINT))
#define IPC_ENDPOINT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), IPC_TYPE_ENDPOINT, IpcEndpointClass))
typedef struct _IpcEndpointClass IpcEndpointClass;

#if __cplusplus
extern "C" {
#endif // __cplusplus

  struct _IpcEndpoint
    {
      GObject parent;
      GHashTable* handlers;
    };

  struct _IpcEndpointClass
    {
      GObjectClass parent;
      void (*emit) (IpcEndpoint* endpoint, const gchar* name, GVariant* params);
    };

  GType ipc_endpoint_get_type (void) G_GNUC_CONST;

  void ipc_endpoint_add_handler (IpcEndpoint* endpoint, IpcHandler* handler, const gchar* name, const GVariantType* param_type);
  void ipc_endpoint_delete_handler (IpcEndpoint* endpoint, const gchar* name);
  void ipc_endpoint_emit (IpcEndpoint* endpoint, const gchar* name, GVariant* params);
  void ipc_endpoint_handle (IpcEndpoint* endpoint, const gchar* name, GVariant* params, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data);
  GVariant* ipc_endpoint_handle_finish (IpcEndpoint* endpoint, GAsyncResult* res, GError** error);
  IpcEndpoint* ipc_endpoint_new (void);

#if __cplusplus
}
#endif // __cplusplus

#endif // __IPC_ENDPOINT__
