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
#include <ipccall.h>
#include <proxyhandlercall.h>

GVariant* ipc_proxy_handler_get_property_pack (const gchar* property_name)
{

  GVariant* items [1] = { g_variant_new_string (property_name) };
  GVariant* tuple = g_variant_new_tuple (items, G_N_ELEMENTS (items));
  GVariant* call = ipc_call_pack ("$get", tuple);
return call;
}

GVariant* ipc_proxy_handler_set_property_pack (const gchar* property_name, GVariant* value)
{
  GVariant* items [2] = { g_variant_new_string (property_name), value };
  GVariant* tuple = g_variant_new_tuple (items, G_N_ELEMENTS (items));
  GVariant* call = ipc_call_pack ("$set", tuple);
return call;
}