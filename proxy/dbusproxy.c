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
#include <report.h>

struct _Step
{
  GAsyncReadyCallback callback;
  gpointer user_data;
};

#define _g_variant_unref0(var) ((var == NULL) ? NULL : (var = (g_variant_unref (var), NULL)))

void _g_dbus_proxy_get_property (GDBusProxy* proxy, const gchar* property_name, gint timeout, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data);
void _g_dbus_proxy_get_property_r (GDBusProxy* proxy, const gchar* property_name, gint timeout, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data);
void _g_dbus_proxy_get_property_r_complete (GObject* _proxy, GAsyncResult* res, gpointer user_data);
void _g_dbus_proxy_set_property_complete (GObject* _proxy, GAsyncResult* res, gpointer user_data);

void _g_dbus_proxy_get_property (GDBusProxy* proxy, const gchar* property_name, gint timeout, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{

  gpointer source_tag = tag (_g_dbus_proxy_get_property);
  GVariant* result;

  if ((result = g_dbus_proxy_get_cached_property (proxy, property_name)) == NULL)

    _g_dbus_proxy_get_property_r (proxy, property_name, timeout, cancellable, callback, user_data);
  else
    _g_task_report_pointer (proxy, callback, user_data, source_tag, result, (GDestroyNotify) g_variant_unref);
}

void _g_dbus_proxy_get_property_r (GDBusProxy* proxy, const gchar* property_name, gint timeout, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{
  GDBusCallFlags flags = G_DBUS_CALL_FLAGS_ALLOW_INTERACTIVE_AUTHORIZATION;
  GDBusInterfaceInfo* info;
  g_return_if_fail ((info = g_dbus_proxy_get_interface_info (proxy)));

  const gchar* method_name = "org.freedesktop.DBus.Properties.Get";
  const gchar* interface_name = info->name;

  GVariant* items [] = { g_variant_new_string (interface_name), g_variant_new_string (property_name) };
  GVariant* parameters = g_variant_new_tuple (items, G_N_ELEMENTS (items));

  struct _Step data = { .callback = callback, .user_data = user_data };
  struct _Step* pdata = g_slice_copy (sizeof (data), &data);

  g_dbus_proxy_call (proxy, method_name, parameters, flags, timeout, cancellable, _g_dbus_proxy_get_property_r_complete, pdata);
}

void _g_dbus_proxy_get_property_r_complete (GObject* _proxy, GAsyncResult* res, gpointer user_data)
{
  struct _Step* pdata = (struct _Step*) user_data;
  GVariant* result = NULL;
  gpointer source_tag = tag (_g_dbus_proxy_get_property);
  GError* tmperr = NULL;

  if ((result = g_dbus_proxy_call_finish ((GDBusProxy*) _proxy, res, &tmperr)), G_UNLIKELY (tmperr != NULL))

    g_task_report_error (_proxy, pdata->callback, pdata->user_data, source_tag, tmperr);
  else
    { GVariant* inner;
      g_variant_get (result, "(v)", &inner);

      _g_task_report_pointer (_proxy, pdata->callback, pdata->user_data, source_tag, inner, (GDestroyNotify) g_variant_unref); }

  g_slice_free (struct _Step, pdata);
  g_variant_unref (result);
}

GVariant* _g_dbus_proxy_get_property_finish (GDBusProxy* proxy G_GNUC_UNUSED, GAsyncResult* res, GError** error)
{
  return g_task_propagate_pointer (G_TASK (res), error);
}

void _g_dbus_proxy_set_property (GDBusProxy* proxy, const gchar* property_name, GVariant* value, gint timeout, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data)
{
  GDBusCallFlags flags = G_DBUS_CALL_FLAGS_ALLOW_INTERACTIVE_AUTHORIZATION;
  GDBusInterfaceInfo* info;
  g_return_if_fail ((info = g_dbus_proxy_get_interface_info (proxy)));

  const gchar* method_name = "org.freedesktop.DBus.Properties.Set";
  const gchar* interface_name = info->name;

  GVariant* items [] = { g_variant_new_string (interface_name), g_variant_new_string (property_name), g_variant_new_variant (value) };
  GVariant* parameters = g_variant_new_tuple (items, G_N_ELEMENTS (items));

  struct _Step data = { .callback = callback, .user_data = user_data };
  struct _Step* pdata = g_slice_copy (sizeof (data), &data);

  g_dbus_proxy_call (proxy, method_name, parameters, flags, timeout, cancellable, _g_dbus_proxy_set_property_complete, pdata);
}

void _g_dbus_proxy_set_property_complete (GObject* _proxy, GAsyncResult* res, gpointer user_data)
{
  struct _Step* pdata = (struct _Step*) user_data;
  GVariant* result = NULL;
  gpointer source_tag = tag (_g_dbus_proxy_set_property);
  GError* tmperr = NULL;

  if ((result = g_dbus_proxy_call_finish ((GDBusProxy*) _proxy, res, &tmperr)), G_UNLIKELY (tmperr != NULL))

    g_task_report_error (_proxy, pdata->callback, pdata->user_data, source_tag, tmperr);
  else
    _g_task_report_boolean (_proxy, pdata->callback, pdata->user_data, source_tag, TRUE);

  g_slice_free (struct _Step, pdata);
  _g_variant_unref0 (result);
}

gboolean _g_dbus_proxy_set_property_finish (GDBusProxy* proxy G_GNUC_UNUSED, GAsyncResult* res, GError** error)
{
  return g_task_propagate_boolean (G_TASK (res), error);
}