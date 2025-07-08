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
#include <dbusproxy.h>
#include <dbusproxyhandler.h>
#include <ipccall.h>
#include <ipchandler.h>
#include <report.h>
#include <proxyhandlererror.h>

struct _IpcDBusProxyHandlerPrivate
{
  GHashTable* properties;
  GDBusInterfaceInfo* info;
  GDBusProxy* proxy;
  gint timeout;
};

enum
{
  prop_0,
  prop_proxiee,
  prop_timeout,
  prop_number,
};

static void ipc_dbus_proxy_handler_ipc_handler_iface (IpcHandlerIface* iface);

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
static GParamSpec* properties [prop_number] = {0};

G_DEFINE_TYPE_WITH_CODE (IpcDBusProxyHandler, ipc_dbus_proxy_handler, G_TYPE_OBJECT,

  G_ADD_PRIVATE (IpcDBusProxyHandler);
  G_IMPLEMENT_INTERFACE (IPC_TYPE_HANDLER, ipc_dbus_proxy_handler_ipc_handler_iface))

static void on_g_signal (IpcDBusProxyHandler* self, const gchar* sender_name G_GNUC_UNUSED, const gchar* signal_name, GVariant* params)
{
  ipc_handler_emit (IPC_HANDLER (self), ipc_call_pack (signal_name, params));
}

static void ipc_dbus_proxy_handler_class_constructed (GObject* pself)
{
  G_OBJECT_CLASS (ipc_dbus_proxy_handler_parent_class)->constructed (pself);
  IpcDBusProxyHandler* self = (IpcDBusProxyHandler*) pself;
  IpcDBusProxyHandlerPrivate* priv = self->priv;
  GDBusInterfaceInfo* info = NULL;

  if ((info = g_dbus_proxy_get_interface_info (self->priv->proxy)) != NULL)
    {
      info = g_dbus_interface_info_ref (info);
      info = (g_dbus_interface_info_cache_build (info), info);
    }

  priv->info = info;
  g_signal_connect_object (priv->proxy, "g-signal", G_CALLBACK (on_g_signal), self, G_CONNECT_SWAPPED);
}

static void ipc_dbus_proxy_handler_class_dispose (GObject* pself)
{
  g_hash_table_remove_all (((IpcDBusProxyHandler*) pself)->priv->properties);
  _g_object_unref0 (((IpcDBusProxyHandler*) pself)->priv->proxy);
G_OBJECT_CLASS (ipc_dbus_proxy_handler_parent_class)->dispose (pself);
}

static void ipc_dbus_proxy_handler_class_finalize (GObject* pself)
{
  IpcDBusProxyHandler* self = (IpcDBusProxyHandler*) pself;
  GDBusInterfaceInfo* info;

  g_hash_table_unref (self->priv->properties);

  if ((info = self->priv->info) != NULL)
    {
      g_dbus_interface_info_cache_release (info);
      g_dbus_interface_info_unref (info);
    }

G_OBJECT_CLASS (ipc_dbus_proxy_handler_parent_class)->dispose (pself);
}

static void ipc_dbus_proxy_handler_class_get_property (GObject* pself, guint property_id, GValue* value, GParamSpec* pspec)
{
  IpcDBusProxyHandler* self = (IpcDBusProxyHandler*) pself;

  switch (property_id)
    {
      case prop_proxiee: g_value_set_object (value, self->priv->proxy);
        break;
      case prop_timeout: g_value_set_int (value, self->priv->timeout);
        break;
      default: G_OBJECT_WARN_INVALID_PROPERTY_ID (pself, property_id, pspec);
        break;
    }
}

static void ipc_dbus_proxy_handler_class_set_property (GObject* pself, guint property_id, const GValue* value, GParamSpec* pspec)
{
  IpcDBusProxyHandler* self = (IpcDBusProxyHandler*) pself;

  switch (property_id)
    {
      case prop_proxiee: g_set_object (&self->priv->proxy, g_value_get_object (value));
        break;
      case prop_timeout: self->priv->timeout = g_value_get_int (value);
        break;
      default: G_OBJECT_WARN_INVALID_PROPERTY_ID (pself, property_id, pspec);
        break;
    }
}

static void ipc_dbus_proxy_handler_class_init (IpcDBusProxyHandlerClass* klass)
{
  GParamFlags flags1 = G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS;
  GParamFlags flags2 = G_PARAM_CONSTRUCT | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS;

  G_OBJECT_CLASS (klass)->constructed = ipc_dbus_proxy_handler_class_constructed;
  G_OBJECT_CLASS (klass)->dispose = ipc_dbus_proxy_handler_class_dispose;
  G_OBJECT_CLASS (klass)->finalize = ipc_dbus_proxy_handler_class_finalize;
  G_OBJECT_CLASS (klass)->get_property = ipc_dbus_proxy_handler_class_get_property;
  G_OBJECT_CLASS (klass)->set_property = ipc_dbus_proxy_handler_class_set_property;

  properties [prop_proxiee] = g_param_spec_object ("proxiee", "proxiee", "proxiee", G_TYPE_DBUS_PROXY, flags1);
  properties [prop_timeout] = g_param_spec_int ("timeout", "timeout", "timeout", -1, G_MAXINT, -1, flags2);
  g_object_class_install_properties (G_OBJECT_CLASS (klass), prop_number, properties);
}

static void ipc_dbus_proxy_handler_init (IpcDBusProxyHandler* self)
{
  IpcDBusProxyHandlerPrivate* priv;

  self->priv = (priv = ipc_dbus_proxy_handler_get_instance_private (self));
  priv->properties = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);
}

static void ipc_dbus_proxy_handler_ipc_handler_iface_handle (IpcHandler* pself, GVariant* params, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data);

static const gchar* guard_name (IpcDBusProxyHandler* self, const gchar* name, GAsyncReadyCallback callback, gpointer user_data)
{
  GDBusInterfaceInfo* info = NULL;
  const gchar* signature = NULL;
  IpcDBusProxyHandlerPrivate* priv = self->priv;
  GDBusPropertyInfo* prop = NULL;

  if (NULL != (info = priv->info) && NULL != (prop = g_dbus_interface_info_lookup_property (info, name)))

    signature = prop->signature;
  else
    {
      const guint code = IPC_PROXY_HANDLER_ERROR_UNKNOWN_PROPERTY;
      const GQuark domain = IPC_PROXY_HANDLER_ERROR;
      const gpointer source_tag = tag (ipc_dbus_proxy_handler_ipc_handler_iface_handle);

      g_task_report_new_error (self, callback, user_data, source_tag, domain, code, "unknown property '%s'", name);
    }
return signature;
}

#define peak_name(v) (G_GNUC_EXTENSION ({ \
 ; \
    GVariant* __p = (v); \
    GVariant* __v = g_variant_get_child_value (__p, 0); \
    const gchar* __n = g_variant_get_string (__v, NULL); \
    g_variant_unref (__v); __n; \
  }))

static void ipc_dbus_proxy_handler_ipc_handler_iface_handle (IpcHandler* pself, GVariant* params, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{
  GDBusCallFlags flags1 = G_DBUS_CALL_FLAGS_ALLOW_INTERACTIVE_AUTHORIZATION;
  GDBusCallFlags flags2 = G_DBUS_CALL_FLAGS_NO_AUTO_START;
  GDBusCallFlags flags = flags1 | flags2;
  GVariant* parameters = NULL;
  IpcDBusProxyHandler* self = (IpcDBusProxyHandler*) pself;
  IpcDBusProxyHandlerPrivate* priv = self->priv;
  gchar* method_name = ipc_call_unpack (params, &parameters);

  do { if (g_str_equal ("$get", method_name))
    {
      const gchar* prop_name;

      if (NULL == guard_name (self, prop_name = peak_name (parameters), callback, user_data))
        break;

      _g_dbus_proxy_get_property (priv->proxy, prop_name, priv->timeout, cancellable, callback, user_data);
      break;
    }
  else if (g_str_equal ("$set", method_name))
    {
      const gchar* prop_name;
      const gchar* signature;
      GVariant* variant;

      if (NULL == (signature = guard_name (self, prop_name = peak_name (parameters), callback, user_data)))
        break;

      variant = g_variant_get_child_value (parameters, 1);

      if (! g_variant_type_equal (signature, g_variant_get_type (variant)))
        {
          const guint code = IPC_PROXY_HANDLER_ERROR_INVALID_SIGNATURE;
          const GQuark domain = IPC_PROXY_HANDLER_ERROR;
          const gpointer source_tag = tag (ipc_dbus_proxy_handler_ipc_handler_iface_handle);

          const gchar* vtype1 = signature;
          const gchar* vtype2 = g_variant_get_type_string (variant);

          g_task_report_new_error (self, callback, user_data, source_tag, domain, code, "invalid signature '%s' (expected '%s')", vtype2, vtype1);
          break;
        }

      _g_dbus_proxy_set_property (priv->proxy, prop_name, variant, priv->timeout, cancellable, callback, user_data);
      g_variant_unref (variant);
      break;
    }
  else { g_dbus_proxy_call (priv->proxy, method_name, parameters, flags, priv->timeout, cancellable, callback, user_data);
    break; } }
  while (TRUE);
  
  g_free (method_name);
  g_variant_unref (parameters);
}

static GVariant* ipc_dbus_proxy_handler_ipc_handler_iface_handle_finish (IpcHandler* pself, GAsyncResult* res, GError** error)
{
  IpcDBusProxyHandler* self = (IpcDBusProxyHandler*) pself;
  IpcDBusProxyHandlerPrivate* priv = self->priv;

  if (g_async_result_is_tagged (res, tag (ipc_dbus_proxy_handler_ipc_handler_iface_handle)))

    return g_task_propagate_pointer (G_TASK (res), error);
  else if (g_async_result_is_tagged (res, tag (_g_dbus_proxy_get_property)))
    {
      GVariant* variant = _g_dbus_proxy_get_property_finish (priv->proxy, res, error);
      GVariant* items [] = { variant };
      GVariant* result = g_variant_new_tuple (items, G_N_ELEMENTS (items));
      return (g_variant_unref (variant), g_variant_take_ref (result));
    }
  else if (g_async_result_is_tagged (res, tag (_g_dbus_proxy_set_property)))
    {
      gboolean good = _g_dbus_proxy_set_property_finish (priv->proxy, res, error);
      GVariant* items [] = { g_variant_new_boolean (good) };
      GVariant* result = g_variant_new_tuple (items, G_N_ELEMENTS (items));
      return g_variant_take_ref (result);
    }
return g_dbus_proxy_call_finish (priv->proxy, res, error);
}

static void ipc_dbus_proxy_handler_ipc_handler_iface (IpcHandlerIface* iface)
{
  iface->handle = ipc_dbus_proxy_handler_ipc_handler_iface_handle;
  iface->handle_finish = ipc_dbus_proxy_handler_ipc_handler_iface_handle_finish;
}

GDBusProxy* ipc_dbus_proxy_handler_get_proxiee (IpcDBusProxyHandler* handler)
{
  g_return_val_if_fail (IPC_IS_DBUS_PROXY_HANDLER (handler), NULL);
  return handler->priv->proxy;
}

gint ipc_dbus_proxy_handler_get_timeout (IpcDBusProxyHandler* handler)
{
  g_return_val_if_fail (IPC_IS_DBUS_PROXY_HANDLER (handler), -1);
  return handler->priv->timeout;
}

IpcDBusProxyHandler* ipc_dbus_proxy_handler_new (GDBusProxy* proxiee)
{
  g_return_val_if_fail (G_IS_DBUS_PROXY (proxiee), NULL);
  return (IpcDBusProxyHandler*) g_object_new (IPC_TYPE_DBUS_PROXY_HANDLER, "proxiee", proxiee, NULL);
}

void ipc_dbus_proxy_handler_set_timeout (IpcDBusProxyHandler* handler, gint timeout_ms)
{
  g_return_if_fail (IPC_IS_DBUS_PROXY_HANDLER (handler));
  handler->priv->timeout = timeout_ms;
}
