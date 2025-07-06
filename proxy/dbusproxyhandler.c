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
#include <dbusproxyhandler.h>
#include <ipccall.h>
#include <ipchandler.h>

struct _IpcDBusProxyHandlerPrivate
{
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
  GDBusProxy* proxy = self->priv->proxy;

  g_signal_connect_object (proxy, "g-signal", G_CALLBACK (on_g_signal), self, G_CONNECT_SWAPPED);
}

static void ipc_dbus_proxy_handler_class_dispose (GObject* pself)
{
  _g_object_unref0 (((IpcDBusProxyHandler*) pself)->priv->proxy);
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
  G_OBJECT_CLASS (klass)->get_property = ipc_dbus_proxy_handler_class_get_property;
  G_OBJECT_CLASS (klass)->set_property = ipc_dbus_proxy_handler_class_set_property;

  properties [prop_proxiee] = g_param_spec_object ("proxiee", "proxiee", "proxiee", G_TYPE_DBUS_PROXY, flags1);
  properties [prop_timeout] = g_param_spec_int ("timeout", "timeout", "timeout", -1, G_MAXINT, -1, flags2);
  g_object_class_install_properties (G_OBJECT_CLASS (klass), prop_number, properties);
}

static void ipc_dbus_proxy_handler_init (IpcDBusProxyHandler* self)
{
  self->priv = (IpcDBusProxyHandlerPrivate*) ipc_dbus_proxy_handler_get_instance_private (self);
}

static void ipc_dbus_proxy_handler_ipc_handler_iface_handle (IpcHandler* pself, GVariant* params, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{
  GDBusCallFlags flags1 = G_DBUS_CALL_FLAGS_ALLOW_INTERACTIVE_AUTHORIZATION;
  GDBusCallFlags flags2 = G_DBUS_CALL_FLAGS_NO_AUTO_START;
  GDBusCallFlags flags = flags1 | flags2;
  GVariant* parameters = NULL;
  GDBusProxy* proxy = ((IpcDBusProxyHandler*) pself)->priv->proxy;
  gint timeout = ((IpcDBusProxyHandler*) pself)->priv->timeout;
  gchar* method_name = ipc_call_unpack (params, &parameters);

  g_dbus_proxy_call (proxy, method_name, parameters, flags, timeout, cancellable, callback, user_data);
  g_variant_unref (parameters);
}

static GVariant* ipc_dbus_proxy_handler_ipc_handler_iface_handle_finish (IpcHandler* pself, GAsyncResult* res, GError** error)
{
  GDBusProxy* proxy = ((IpcDBusProxyHandler*) pself)->priv->proxy;
  return g_dbus_proxy_call_finish (proxy, res, error);
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
