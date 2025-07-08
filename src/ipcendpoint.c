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
#include <ipcendpoint.h>
#include <ipcmarshal.h>
#include <ipcreplay.h>
#include <report.h>

struct _Handler
{
  IpcHandler* handler;
  gchar* name;
  GVariantType* param_type;
  guint signal_id;
};

struct _HandleData
{
  gchar* name;
  GVariant* params;
};

struct _Step
{
  IpcEndpoint* self;
  GAsyncReadyCallback callback;
  gpointer user_data;
};

enum
{
  sig_emit,
  sig_number,
};


G_DEFINE_TYPE (IpcEndpoint, ipc_endpoint, G_TYPE_OBJECT)
static guint signals [sig_number] = {0};

#define tag(t) (G_GNUC_EXTENSION ({ (gpointer) ((t)); }))

static void _handler_free (gpointer ptr)
{
  struct _Handler* data = (struct _Handler*) ptr;

  g_signal_handler_disconnect (data->handler, data->signal_id);
  g_object_unref (data->handler);
  g_free (data->name);
  g_variant_type_free (data->param_type);
  g_slice_free (struct _Handler, ptr);
}

static void ipc_endpoint_init (IpcEndpoint* self)
{
  self->handlers = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, _handler_free);
}

static void ipc_endpoint_class_dispose (GObject* pself)
{
  GHashTableIter iter;
  struct _Handler* handler;

  g_hash_table_iter_init (&iter, ((IpcEndpoint*) pself)->handlers);

  while (g_hash_table_iter_next (&iter, NULL, (gpointer*) &handler))
    {
      g_signal_handler_disconnect (handler->handler, handler->signal_id);
    }

  g_hash_table_remove_all (((IpcEndpoint*) pself)->handlers);
  G_OBJECT_CLASS (ipc_endpoint_parent_class)->dispose (pself);
}

static void ipc_endpoint_class_finalize (GObject* pself)
{
  g_hash_table_unref (((IpcEndpoint*) pself)->handlers);
  G_OBJECT_CLASS (ipc_endpoint_parent_class)->finalize (pself);
}

static void ipc_endpoint_class_init (IpcEndpointClass* klass)
{
  G_OBJECT_CLASS (klass)->dispose = ipc_endpoint_class_dispose;
  G_OBJECT_CLASS (klass)->finalize = ipc_endpoint_class_finalize;

  const GSignalCMarshaller c_marshaller = g_cclosure_user_marshal_VOID__STRING_VARIANT;
  const goffset class_offset = G_STRUCT_OFFSET (IpcEndpointClass, emit);
  const GType itype = G_TYPE_FROM_CLASS (klass);
  const GSignalCVaMarshaller va_marshaller = g_cclosure_user_marshal_VOID__STRING_VARIANTv;

  signals [sig_emit] = g_signal_new ("emit", itype, G_SIGNAL_ACTION, class_offset, NULL, NULL, c_marshaller, G_TYPE_NONE, 2, G_TYPE_STRING, G_TYPE_VARIANT);

  g_signal_set_va_marshaller (signals [sig_emit], itype, va_marshaller);
}

struct _EmitData
{
  IpcEndpoint* endpoint;
  gchar* name;
};

static inline void _emit_data_free (struct _EmitData* data, GClosure* closure G_GNUC_UNUSED)
{
  g_free (data->name);
  g_slice_free (struct _EmitData, data);
}

static void on_emit (IpcHandler* handler G_GNUC_UNUSED, GVariant* params, struct _EmitData* data)
{
  ipc_endpoint_emit (data->endpoint, data->name, params);
}

void ipc_endpoint_add_handler (IpcEndpoint* endpoint, IpcHandler* handler, const gchar* name, const GVariantType* param_type)
{
  g_return_if_fail (IPC_IS_ENDPOINT (endpoint));
  g_return_if_fail (IPC_IS_HANDLER (handler));
  g_return_if_fail (name != NULL);
  const gchar* param_type_ = g_variant_type_peek_string (param_type);
  g_return_if_fail (param_type == NULL || g_variant_type_string_is_valid (param_type_));

  struct _EmitData emt =
    {
      .endpoint = endpoint,
      .name = g_strdup (name),
    };

  struct _EmitData* emtp = g_slice_dup (struct _EmitData, &emt);
  const GClosureNotify notify = (GClosureNotify) _emit_data_free;

  struct _Handler reg =
    {
      .handler = g_object_ref (handler),
      .name = g_strdup (name),
      .param_type = g_variant_type_copy (param_type),
      .signal_id = g_signal_connect_data (handler, "emit", G_CALLBACK (on_emit), emtp, notify, 0),
    };

  g_hash_table_insert (endpoint->handlers, g_strdup (name), g_slice_dup (struct _Handler, &reg));
}

void ipc_endpoint_delete_handler (IpcEndpoint* endpoint, const gchar* name)
{
  g_return_if_fail (IPC_IS_ENDPOINT (endpoint));
  g_return_if_fail (name != NULL);

  g_hash_table_remove (endpoint->handlers, name);
}

#define g_value_init_to(value,as,gtype,to) (G_GNUC_EXTENSION ({ \
    ; \
      GType __gtype = (gtype); \
      GValue* __value = (value); \
    ; \
      g_value_init (__value, __gtype); \
      g_value_set_##as (__value, (to)); \
  }))

void ipc_endpoint_emit (IpcEndpoint* endpoint, const gchar* name, GVariant* params)
{
  g_return_if_fail (IPC_IS_ENDPOINT (endpoint));
  g_return_if_fail (name != NULL);
  g_return_if_fail (params != NULL);
  GValue values [3] = {0};

  g_value_init_to (& values [0], object, IPC_TYPE_ENDPOINT, endpoint);
  g_value_init_to (& values [1], string, G_TYPE_STRING, name);
  g_value_init_to (& values [2], variant, G_TYPE_VARIANT, params);

  g_signal_emitv (values, signals [sig_emit], 0, NULL);

  g_value_unset (& values [2]);
  g_value_unset (& values [1]);
  g_value_unset (& values [0]);
}

static void ipc_endpoint_handle_complete (GObject* pself, GAsyncResult* res, gpointer user_data);

void ipc_endpoint_handle (IpcEndpoint* endpoint, const gchar* name, GVariant* params, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{
  g_return_if_fail (IPC_IS_ENDPOINT (endpoint));
  g_return_if_fail (name != NULL);
  g_return_if_fail (params != NULL);
  g_return_if_fail (cancellable == NULL || G_IS_CANCELLABLE (cancellable));

  const gpointer _tag = tag (ipc_endpoint_handle);
  struct _Handler* handler;
  IpcEndpoint* self = (IpcEndpoint*) endpoint;
  GVariantType* param_type = NULL;

  if (! g_hash_table_lookup_extended (self->handlers, name, NULL, (gpointer*) &handler))

    g_task_report_new_error (self, callback, user_data, _tag, G_IO_ERROR, G_IO_ERROR_FAILED, "unknown handler %s", name);
  else

    if ((param_type = handler->param_type) == NULL
        || g_variant_check_format_string (params, g_variant_type_peek_string (param_type), FALSE))
      {
        struct _Step data = { .callback = callback, .self = g_object_ref (endpoint), .user_data = user_data };
        struct _Step* pdata = g_slice_copy (sizeof (data), &data);

        ipc_handler_handle (handler->handler, params, cancellable, ipc_endpoint_handle_complete, pdata);
      }
    else
      {
        const gchar* expected = g_variant_type_peek_string (param_type);
        const gchar* got = g_variant_get_type_string (params);

        g_task_report_new_error (self, callback, user_data, _tag, G_IO_ERROR, G_IO_ERROR_INVALID_ARGUMENT,
                                                                  "invalid param types ('%s' expected, got '%s')", expected, got);
      }
}

static void ipc_endpoint_handle_complete (GObject* pself, GAsyncResult* res, gpointer user_data)
{
  struct _Step* pdata = (struct _Step*) user_data;
  GVariant* result;
  gpointer source_tag = tag (ipc_endpoint_handle);
  GError* tmperr = NULL;

  if ((result = ipc_handler_handle_finish ((IpcHandler*) pself, res, &tmperr)), G_UNLIKELY (tmperr != NULL))

    g_task_report_error (pdata->self, pdata->callback, pdata->user_data, source_tag, tmperr);
  else
    _g_task_report_pointer (pdata->self, pdata->callback, pdata->user_data, source_tag, result, (GDestroyNotify) g_variant_unref);

  g_object_unref (pdata->self);
  g_slice_free (struct _Step, pdata);
}

GVariant* ipc_endpoint_handle_finish (IpcEndpoint* endpoint, GAsyncResult* res, GError** error)
{
  g_return_val_if_fail (IPC_IS_ENDPOINT (endpoint), NULL);
  g_return_val_if_fail (G_IS_ASYNC_RESULT (res), NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);
  const gpointer _tag = tag (ipc_endpoint_handle);

  if (g_async_result_is_tagged (res, _tag))

    return g_task_propagate_pointer (G_TASK (res), error);
  else
    { gpointer handler = g_async_result_get_source_object (res);
      return ipc_handler_handle_finish ((IpcHandler*) handler, res, error); }
}

IpcEndpoint* ipc_endpoint_new (void)
{
  return g_object_new (IPC_TYPE_ENDPOINT, NULL);
}
