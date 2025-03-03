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
#ifndef __IPC_MESSAGE_HANDLER__
#define __IPC_MESSAGE_HANDLER__
#include <gio/gio.h>

#define IPC_TYPE_HANDLER (ipc_handler_get_type ())
#define IPC_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), IPC_TYPE_HANDLER, IpcHandler))
#define IPC_IS_HANDLER(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), IPC_TYPE_HANDLER))
#define IPC_HANDLER_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE((obj), IPC_TYPE_HANDLER, IpcHandlerIface))

typedef struct _IpcHandler IpcHandler;
typedef struct _IpcHandlerIface IpcHandlerIface;

#if __cplusplus
extern "C" {
#endif // __cplusplus

  struct _IpcHandlerIface
    {
      GTypeInterface parent;
      void (*emit) (IpcHandler* handler, GVariant* params);
      void (*handle) (IpcHandler* handler, GVariant* params, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data);
      GVariant* (*handle_finish) (IpcHandler* handler, GAsyncResult* res, GError** error);
    };

  GType ipc_handler_get_type (void) G_GNUC_CONST;
  void ipc_handler_default_handle (IpcHandler* handler, GVariant* params, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data);
  GVariant* ipc_handler_default_handle_finish (IpcHandler* handler, GAsyncResult* res, GError** error);
  void ipc_handler_emit (IpcHandler* handler, GVariant* params);
  void ipc_handler_handle (IpcHandler* handler, GVariant* params, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data);
  GVariant *ipc_handler_handle_finish (IpcHandler *handler, GAsyncResult* res, GError** error);

#if __cplusplus
}
#endif // __cplusplus

#endif // __IPC_MESSAGE_HANDLER__
