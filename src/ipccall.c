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

#define _g_variant_unref0(var) ((var == NULL) ? NULL : (var = (g_variant_unref (var), NULL)))

GVariant* ipc_call_pack (const gchar* name, GVariant* arguments)
{
  g_return_val_if_fail (name != NULL, NULL);
  g_return_val_if_fail (arguments != NULL, NULL);

  GVariant* values [] =
    {
      g_variant_new_string (name),
      arguments,
    };

  return g_variant_new_tuple (values, G_N_ELEMENTS (values));
}

gchar* ipc_call_unpack (GVariant* call, GVariant** arguments)
{
  g_return_val_if_fail (call != NULL, NULL);
  GVariant* placeholder = NULL;
  gchar* name = NULL;

  g_variant_get (call, "(sr)", &name, arguments ? arguments : &placeholder);
  return (_g_variant_unref0 (placeholder), name);
}
