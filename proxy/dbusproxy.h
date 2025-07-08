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
#ifndef __IPC_DBUS_PROXY__
#define __IPC_DBUS_PROXY__ 1
#include <gio/gio.h>

#if __cplusplus
extern "C" {
#endif // __cplusplus

  G_GNUC_INTERNAL void _g_dbus_proxy_get_property (GDBusProxy* proxy, const gchar* name, gint timeout, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data);
  G_GNUC_INTERNAL GVariant* _g_dbus_proxy_get_property_finish (GDBusProxy* proxy, GAsyncResult* res, GError** error);
  G_GNUC_INTERNAL void _g_dbus_proxy_set_property (GDBusProxy* proxy, const gchar* name, GVariant* value, gint timeout, GCancellable* cancellable, GAsyncReadyCallback callback, gpointer user_data);
  G_GNUC_INTERNAL gboolean _g_dbus_proxy_set_property_finish (GDBusProxy* proxy, GAsyncResult* res, GError** error);

#if __cplusplus
}
#endif // __cplusplus

#endif // __IPC_DBUS_PROXY__