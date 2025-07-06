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

static GVariant* val2arr (GValue* value, const GVariantType* ctype)
{
  const GVariantType* itype;

  switch (((const gchar*) (itype = g_variant_type_element (ctype))) [0])
    {

      case 's': { GStrv strv = g_value_get_boxed (value);
                  return strv == NULL ? NULL : g_variant_new_strv ((const gchar* const*) strv, -1); }

      case 'y': { GBytes* bytes = g_value_get_boxed (value);
                  return bytes == NULL ? NULL : g_variant_new_from_bytes (ctype, bytes, TRUE); }

      default: g_assert_not_reached ();
    }
}

static const GVariantType* gtype2vtype (GType gtype, gboolean nullable)
{

  switch (g_type_fundamental (gtype))
    {

      case G_TYPE_BOOLEAN: return (GVariantType*) "b";

      case G_TYPE_BOXED:
        {
          if (gtype == G_TYPE_BYTES)
            return (GVariantType*) (! nullable ? "ay" : "may");
          else if (gtype == G_TYPE_STRV)
            return (GVariantType*) (! nullable ? "as" : "mas");
          else return NULL;
        }

      case G_TYPE_DOUBLE: return (GVariantType*) "d";
      case G_TYPE_ENUM: return (GVariantType*) "i";
      case G_TYPE_FLAGS: return (GVariantType*) "u";
      case G_TYPE_INT: return (GVariantType*) "i";
      case G_TYPE_INT64: return (GVariantType*) "x";
      case G_TYPE_STRING: return (GVariantType*) (! nullable ? "s" : "ms");
      case G_TYPE_UCHAR: return (GVariantType*) "y";
      case G_TYPE_UINT: return (GVariantType*) "u";
      case G_TYPE_UINT64: return (GVariantType*) "t";
      case G_TYPE_VARIANT: return (GVariantType*) "v";
      default: return NULL;
    }
}

static gboolean should_nullable (GType gtype, GValue* value)
{
  switch (g_type_fundamental (gtype))
    {
      case G_TYPE_BOXED: return NULL == g_value_get_boxed (value);
      case G_TYPE_STRING: return NULL == g_value_get_string (value);
      default: return FALSE;
    }
}

GVariant* _ipc_convert_gvalue_to_gvariant (GValue* value, GError** error)
{
  const GType gtype = G_VALUE_TYPE (value);
  const GVariantType* vtype = gtype2vtype (gtype, should_nullable (gtype, value));

  if ((vtype = gtype2vtype (gtype, should_nullable (gtype, value))) == NULL)
    {
      GQuark domain = IPC_PROXY_HANDLER_ERROR;
      guint code = IPC_PROXY_HANDLER_ERROR_UNKNOWN_CONVERSION;

      g_set_error (error, domain, code, "Unknown conversion for type '%s'", g_type_name (gtype));
      return NULL;
    }

  if (g_variant_type_is_maybe (vtype))
    {

      const GVariantType* ctype;

      switch (((const gchar*) (ctype = g_variant_type_element (vtype))) [0])
        {

          case 'a': { return g_variant_new_maybe (ctype, val2arr (value, ctype)); }

          case 's': { const gchar* _value = g_value_get_string (value);
                      return g_variant_new_maybe (ctype, _value == NULL ? NULL : g_variant_new_string (_value)); }

          default: g_assert_not_reached ();
        }
    }

  switch (((const gchar*) vtype) [0])
    {

      case 'a': { return val2arr (value, vtype); }
      case 'b': { return g_variant_new_boolean (g_value_get_boolean (value)); }
      case 'd': { return g_variant_new_double (g_value_get_double (value)); }
      case 'i': { return g_variant_new_int32 (g_value_get_int (value)); }
      case 's': { return g_variant_new_string (g_value_get_string (value)); }
      case 't': { return g_variant_new_uint64 (g_value_get_uint64 (value)); }
      case 'u': { return g_variant_new_uint32 (g_value_get_uint (value)); }
      case 'v': { return g_variant_new_variant (g_value_get_variant (value)); }
      case 'x': { return g_variant_new_int64 (g_value_get_int64 (value)); }
      case 'y': { return g_variant_new_byte (g_value_get_uchar (value)); }
      default: g_assert_not_reached ();
    }
}