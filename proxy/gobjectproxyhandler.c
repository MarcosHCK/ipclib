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
#include <convert.h>
#include <ipccall.h>
#include <ipchandler.h>
#include <gobjectproxyhandler.h>
#include <report.h>
#include <proxyhandlererror.h>

struct _IpcGObjectProxyHandlerPrivate
{
  GHashTable* properties;
  GObject* proxiee;
};

enum
{
  prop_0,
  prop_proxiee,
  prop_number,
};

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
static void ipc_gobject_proxy_handler_ipc_handler_iface (IpcHandlerIface* iface);
static GParamSpec* properties [prop_number] = { 0 };

G_DEFINE_TYPE_WITH_CODE (IpcGObjectProxyHandler, ipc_gobject_proxy_handler, G_TYPE_OBJECT,
G_ADD_PRIVATE (IpcGObjectProxyHandler)
G_IMPLEMENT_INTERFACE (IPC_TYPE_HANDLER, ipc_gobject_proxy_handler_ipc_handler_iface))

static void ipc_gobject_proxy_handler_class_constructed (GObject* pself)
{
  IpcGObjectProxyHandlerPrivate* priv = ((IpcGObjectProxyHandler*) pself)->priv;
G_OBJECT_CLASS (ipc_gobject_proxy_handler_parent_class)->constructed (pself);

  GObjectClass* klass = G_OBJECT_GET_CLASS (priv->proxiee);

  guint i, n_properties = 0;
  GParamSpec** properties = g_object_class_list_properties (klass, &n_properties);

  for (i = 0; i < n_properties; ++i)
    {
      gchar* key = g_strdup (properties [i]->name);
      gchar* value = (gchar*) properties [i];

      g_hash_table_insert (priv->properties, (gpointer) key, (gpointer) value);
    }
}

static void ipc_gobject_proxy_handler_class_dispose (GObject* pself)
{
  IpcGObjectProxyHandlerPrivate* priv = ((IpcGObjectProxyHandler*) pself)->priv;

  g_hash_table_remove_all (priv->properties);
  _g_object_unref0 (priv->proxiee);
G_OBJECT_CLASS (ipc_gobject_proxy_handler_parent_class)->dispose (pself);
}

static void ipc_gobject_proxy_handler_class_finalize (GObject* pself)
{
  IpcGObjectProxyHandlerPrivate* priv = ((IpcGObjectProxyHandler*) pself)->priv;

  g_hash_table_unref (priv->properties);
G_OBJECT_CLASS (ipc_gobject_proxy_handler_parent_class)->finalize (pself);
}

static void ipc_gobject_proxy_handler_class_get_property (GObject* pself, guint property_id, GValue* value, GParamSpec* pspec)
{
  IpcGObjectProxyHandlerPrivate* priv = ((IpcGObjectProxyHandler*) pself)->priv;

  switch (property_id)
    {
      case prop_proxiee: g_value_set_object (value, priv->proxiee);
        break;
      default: G_OBJECT_WARN_INVALID_PROPERTY_ID (pself, property_id, pspec);
        break;
    }
}

static void ipc_gobject_proxy_handler_class_set_property (GObject* pself, guint property_id, const GValue* value, GParamSpec* pspec)
{
  IpcGObjectProxyHandlerPrivate* priv = ((IpcGObjectProxyHandler*) pself)->priv;

  switch (property_id)
    {
      case prop_proxiee: g_set_object (&priv->proxiee, g_value_get_object (value));
        break;
      default: G_OBJECT_WARN_INVALID_PROPERTY_ID (pself, property_id, pspec);
        break;
    }
}

static void ipc_gobject_proxy_handler_class_invoke (IpcGObjectProxyHandler* self, const gchar* method, GVariant* parameters G_GNUC_UNUSED, GCancellable* cancellable G_GNUC_UNUSED, GAsyncReadyCallback callback, gpointer user_data)
{
  const guint code = IPC_PROXY_HANDLER_ERROR_UNKNOWN_METHOD;
  const GQuark domain = IPC_PROXY_HANDLER_ERROR;

  g_task_report_new_error (self, callback, user_data, tag (ipc_gobject_proxy_handler_class_invoke), domain, code, "unknown method '%s'", method);
}

static GVariant* ipc_gobject_proxy_handler_class_invoke_finish (IpcGObjectProxyHandler* self G_GNUC_UNUSED, GAsyncResult* res, GError** error)
{
  return g_task_propagate_pointer (G_TASK (res), error);
}

static void ipc_gobject_proxy_handler_class_init (IpcGObjectProxyHandlerClass* klass)
{
  GParamFlags flags1 = G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS;

  G_OBJECT_CLASS (klass)->constructed = ipc_gobject_proxy_handler_class_constructed;
  G_OBJECT_CLASS (klass)->dispose = ipc_gobject_proxy_handler_class_dispose;
  G_OBJECT_CLASS (klass)->finalize = ipc_gobject_proxy_handler_class_finalize;
  G_OBJECT_CLASS (klass)->get_property = ipc_gobject_proxy_handler_class_get_property;
  G_OBJECT_CLASS (klass)->set_property = ipc_gobject_proxy_handler_class_set_property;

  klass->invoke = ipc_gobject_proxy_handler_class_invoke;
  klass->invoke_finish = ipc_gobject_proxy_handler_class_invoke_finish;

  properties [prop_proxiee] = g_param_spec_object ("proxiee", "proxiee", "proxiee", G_TYPE_OBJECT, flags1);
  g_object_class_install_properties (G_OBJECT_CLASS (klass), prop_number, properties);
}

static void ipc_gobject_proxy_handler_init (IpcGObjectProxyHandler* self)
{
  IpcGObjectProxyHandlerPrivate* priv;

  self->priv = (priv = ipc_gobject_proxy_handler_get_instance_private (self));
  priv->properties = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);
}

static void ipc_gobject_proxy_handler_ipc_handler_iface_handle (IpcHandler* pself, GVariant* call, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data);

static GType guard_name (IpcGObjectProxyHandler* self, const gchar* name, GValue* value, GAsyncReadyCallback callback, gpointer user_data)
{
  gboolean have;
  GParamSpec* pspec;

  if ((have = g_hash_table_lookup_extended (self->priv->properties, (gconstpointer) name, NULL, (gpointer*) &pspec)))

    g_value_init (value, pspec->value_type);
  else
    {
      GQuark domain = IPC_PROXY_HANDLER_ERROR;
      guint code = IPC_PROXY_HANDLER_ERROR_UNKNOWN_PROPERTY;

      g_task_report_new_error (self, callback, user_data, tag (ipc_gobject_proxy_handler_ipc_handler_iface_handle), domain, code, "Unknown property '%s'", name);
    }
return have;
}

#define peak_name(v) (G_GNUC_EXTENSION ({ \
 ; \
    GVariant* __p = (v); \
    GVariant* __v = g_variant_get_child_value (__p, 0); \
    const gchar* __n = g_variant_get_string (__v, NULL); \
    g_variant_unref (__v); __n; \
}))

static GVariant* wrap (GVariant* result)
{
  GVariant* items [] = { result };
  GVariant* wrapped = g_variant_new_tuple (items, G_N_ELEMENTS (items));
return g_variant_take_ref (wrapped);
}

static void ipc_gobject_proxy_handler_ipc_handler_iface_handle (IpcHandler* pself, GVariant* call, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{

  GError* tmperr = NULL;
  GVariant* arguments;
  gchar* method = ipc_call_unpack (call, &arguments);
  const gchar* name = NULL;
  IpcGObjectProxyHandler* self = (IpcGObjectProxyHandler*) pself;

  do { if (g_str_equal (method, "$get"))
    {
      GVariant* result = NULL;
      GValue value = G_VALUE_INIT;

      if (! guard_name (self, name = peak_name (arguments), &value, callback, user_data))
        break;

      g_object_get_property (self->priv->proxiee, name, &value);

      if ((result = _ipc_convert_gvalue_to_gvariant (&value, &tmperr)), G_UNLIKELY (tmperr != NULL))
        {
          g_task_report_error (pself, callback, user_data, tag (ipc_gobject_proxy_handler_ipc_handler_iface_handle), tmperr);
          break;
        }

      _g_task_report_pointer (self, callback, user_data, tag (ipc_gobject_proxy_handler_ipc_handler_iface_handle), wrap (result), (GDestroyNotify) g_variant_unref);
      g_value_unset (&value);
      break;
    }
  else if (g_str_equal (method, "$set"))
    {
      GVariant* to = NULL;
      GVariant* result = NULL;
      GValue value = G_VALUE_INIT;

      if (! guard_name (self, name = peak_name (arguments), &value, callback, user_data))
        break;

      if ((_ipc_convert_gvariant_to_gvalue (to = g_variant_get_child_value (arguments, 1), &value, &tmperr)), G_UNLIKELY (tmperr != NULL))
        {
          g_task_report_error (pself, callback, user_data, tag (ipc_gobject_proxy_handler_ipc_handler_iface_handle), tmperr);
          g_variant_unref (to);
          break;
        }

      result = g_variant_new_boolean (TRUE);

      g_object_set_property (self->priv->proxiee, name, &value);
      _g_task_report_pointer (self, callback, user_data, tag (ipc_gobject_proxy_handler_ipc_handler_iface_handle), wrap (result), (GDestroyNotify) g_variant_unref);

      g_value_unset (&value);
      g_variant_unref (to);
      break;
    }
  else { ipc_gobject_proxy_handler_invoke (self, method, arguments, cancellable, callback, user_data);
          break; } }
  while (TRUE);

  g_variant_unref (arguments);
  g_free (method);
}

static GVariant* ipc_gobject_proxy_handler_ipc_handler_iface_handle_finish (IpcHandler* pself, GAsyncResult* res, GError** error)
{

  if (g_async_result_is_tagged (res, tag (ipc_gobject_proxy_handler_ipc_handler_iface_handle)))

    return g_task_propagate_pointer (G_TASK (res), error);
  else
    return ipc_gobject_proxy_handler_invoke_finish ((IpcGObjectProxyHandler*) pself, res, error);
}

static void ipc_gobject_proxy_handler_ipc_handler_iface (IpcHandlerIface* iface)
{
  iface->handle = ipc_gobject_proxy_handler_ipc_handler_iface_handle;
  iface->handle_finish = ipc_gobject_proxy_handler_ipc_handler_iface_handle_finish;
}

GObject* ipc_gobject_proxy_handler_get_proxiee (IpcGObjectProxyHandler* self)
{
  g_return_val_if_fail (IPC_IS_GOBJECT_PROXY_HANDLER (self), NULL);
return self->priv->proxiee;
}

void ipc_gobject_proxy_handler_invoke (IpcGObjectProxyHandler* handler, const gchar* method, GVariant* arguments, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{
  g_return_if_fail (IPC_IS_GOBJECT_PROXY_HANDLER (handler));
  g_return_if_fail (method != NULL);
  g_return_if_fail (arguments != NULL);
  g_return_if_fail (cancellable == NULL || G_IS_CANCELLABLE (cancellable));

  IPC_GOBJECT_PROXY_HANDLER_GET_CLASS (handler)->invoke (handler, method, arguments, cancellable, callback, user_data);
}

GVariant* ipc_gobject_proxy_handler_invoke_finish (IpcGObjectProxyHandler* handler, GAsyncResult* res, GError** error)
{
  g_return_val_if_fail (IPC_IS_GOBJECT_PROXY_HANDLER (handler), NULL);
  g_return_val_if_fail (G_IS_ASYNC_RESULT (res), NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

return IPC_GOBJECT_PROXY_HANDLER_GET_CLASS (handler)->invoke_finish (handler, res, error);
}

IpcGObjectProxyHandler* ipc_gobject_proxy_handler_new (GObject* proxiee)
{
  return (IpcGObjectProxyHandler*) g_object_new (IPC_TYPE_GOBJECT_PROXY_HANDLER, "proxiee", proxiee, NULL);
}