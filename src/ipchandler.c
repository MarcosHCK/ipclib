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
#include <ipchandler.h>

enum
{
  sig_emit,
  sig_number,
};

typedef IpcHandlerIface IpcHandlerInterface;
G_DEFINE_INTERFACE (IpcHandler, ipc_handler, G_TYPE_OBJECT)

static guint signals [sig_number] = {0};

void ipc_handler_default_handle (IpcHandler* handler, GVariant* params, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{
  (void) params;
  (void) cancellable;

  GError* error = g_error_new_literal (G_IO_ERROR, G_IO_ERROR_FAILED, "unimplemented");
  g_task_report_error (handler, callback, user_data, (gpointer) ipc_handler_default_handle, error);
}

GVariant* ipc_handler_default_handle_finish (IpcHandler* handler, GAsyncResult* res, GError** error)
{
  (void) handler;
  return g_task_propagate_pointer (G_TASK (res), error);
}

static void ipc_handler_default_init (IpcHandlerIface* iface)
{
  iface->handle = ipc_handler_default_handle;
  iface->handle_finish = ipc_handler_default_handle_finish;

  const GSignalCMarshaller c_marshaller = g_cclosure_marshal_VOID__VARIANT;
  const goffset class_offset = G_STRUCT_OFFSET (IpcHandlerIface, emit);
  const GType itype = G_TYPE_FROM_INTERFACE (iface);
  const GSignalCVaMarshaller va_marshaller = g_cclosure_marshal_VOID__VARIANTv;

  signals [sig_emit] = g_signal_new ("emit", itype, G_SIGNAL_ACTION, class_offset, NULL, NULL, c_marshaller, G_TYPE_NONE, 1, G_TYPE_VARIANT);

  g_signal_set_va_marshaller (signals [sig_emit], itype, va_marshaller);
}

#define g_value_init_to(value,as,gtype,to) (G_GNUC_EXTENSION ({ \
    ; \
      GType __gtype = (gtype); \
      GValue* __value = (value); \
    ; \
      g_value_init (__value, __gtype); \
      g_value_set_##as (__value, (to)); \
  }))

void ipc_handler_emit (IpcHandler* handler, GVariant* params)
{
  g_return_if_fail (IPC_IS_HANDLER (handler));
  g_return_if_fail (params != NULL);

  GValue values [2] = {0};

  g_value_init_to (& values [0], object, IPC_TYPE_HANDLER, handler);
  g_value_init_to (& values [1], variant, G_TYPE_VARIANT, params);
  g_signal_emitv (values, signals [sig_emit], 0, NULL);

  g_value_unset (& values [1]);
  g_value_unset (& values [0]);
}

void ipc_handler_handle (IpcHandler* handler, GVariant* params, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{
  g_return_if_fail (IPC_IS_HANDLER (handler));
  g_return_if_fail (params != NULL);
  g_return_if_fail (cancellable == NULL || G_IS_CANCELLABLE (cancellable));

  IPC_HANDLER_GET_INTERFACE (handler)->handle (handler, params, cancellable, callback, user_data);
}

GVariant *ipc_handler_handle_finish(IpcHandler *handler, GAsyncResult* res, GError** error)
{
  g_return_val_if_fail (IPC_IS_HANDLER (handler), NULL);
  g_return_val_if_fail (G_IS_TASK (res), NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  return IPC_HANDLER_GET_INTERFACE (handler)->handle_finish (handler, res, error);
}
