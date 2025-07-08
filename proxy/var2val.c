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

#define throw(code,...) (G_GNUC_EXTENSION ({ \
 ; \
    const guint __code = IPC_PROXY_HANDLER_ERROR_##code; \
    const GQuark __domain = IPC_PROXY_HANDLER_ERROR; \
 ; \
    g_set_error (error, __domain, __code, __VA_ARGS__); \
  }))

#define checkfmt(vtype) (G_GNUC_EXTENSION ({ \
 ; \
    const GVariantType* __vtype1 = ((GVariantType*) ((vtype))); \
    const GVariantType* __vtype2 = g_variant_get_type (variant); \
 ; \
    if (! g_variant_type_equal (__vtype1, __vtype2)) goto __error; \
  }))

#define g_variant_get_(suffix,vtype) (G_GNUC_EXTENSION ({ \
 ; \
    checkfmt (vtype); \
    g_variant_get_##suffix (variant); \
  }))

#define var2bytes(v) (g_variant_get_data_as_bytes ((v)))

static GStrv var2strv (GVariant* variant)
{
  GVariantIter iter;
  guint i;
  guint length = g_variant_n_children (variant);
  gchar* next = NULL;
  gchar** strv = g_new (gchar*, 1 + length);

  g_variant_iter_init (&iter, variant);

  for (i = 0; g_variant_iter_next (&iter, "s", &next); ++i)
    strv [i] = next;
return (strv [length] = NULL, strv);
}

void _ipc_convert_gvariant_to_gvalue (GVariant* variant, GValue* value, GError** error)
{

  if (g_variant_is_container (variant))
    {
      const guint code = IPC_PROXY_HANDLER_ERROR_UNKNOWN_CONVERSION;
      const GQuark domain = IPC_PROXY_HANDLER_ERROR;
      const gchar* vtype = g_variant_get_type_string (variant);

      g_set_error (error, domain, code, "unknown conversion for type '%s'", vtype);
      return;
    }

  GType gtype;

  switch (g_type_fundamental (gtype = G_VALUE_TYPE (value)))
    {

      default: __error:
        {
          const gchar* name = g_type_name (gtype);
          throw (UNKNOWN_CONVERSION, "unknown conversion for gtype '%s'", name);
          break;
        }

      case G_TYPE_BOOLEAN: { gboolean _value = g_variant_get_ (boolean, "b");
                             g_value_set_boolean (value, _value); break; }

      case G_TYPE_BOXED:
        {
          if (g_type_is_a (gtype, G_TYPE_BYTES))
            g_value_take_boxed (value, (checkfmt ("ay"), var2bytes (variant)));
          else if (g_type_is_a (gtype, G_TYPE_STRV))
            g_value_take_boxed (value, (checkfmt ("as"), var2strv (variant)));
          else
            { const gchar* name = g_type_name (gtype);
              throw (UNKNOWN_CONVERSION, "unknown conversion for gtype '%s'", name); }
          break;
        }

      case G_TYPE_ENUM: { gint _value = g_variant_get_ (int32, "i");
                          g_value_set_enum (value, _value); break; }

      case G_TYPE_FLAGS: { guint _value = g_variant_get_ (uint32, "u");
                           g_value_set_flags (value, _value); break; }

      case G_TYPE_INT: { gint _value = g_variant_get_ (int32, "i");
                         g_value_set_int (value, _value); break; }

      case G_TYPE_INT64: { gint64 _value = g_variant_get_ (int64, "x");
                           g_value_set_int64 (value, _value); break; }

      case G_TYPE_STRING: { const gchar* _value = (checkfmt ("s"), g_variant_get_string (variant, NULL));
                            g_value_set_string (value, _value); break; }

      case G_TYPE_UCHAR: { guint8 _value = g_variant_get_ (byte, "y");
                           g_value_set_uchar (value, _value); break; }

      case G_TYPE_UINT: { guint _value = g_variant_get_ (uint32, "u");
                          g_value_set_uint (value, _value); break; }

      case G_TYPE_UINT64: { guint64 _value = g_variant_get_ (uint64, "t");
                            g_value_set_uint64 (value, _value); break; }

      case G_TYPE_VARIANT: { GVariant* _value = g_variant_get_ (variant, "v");
                             g_value_set_variant (value, _value);
                             g_variant_unref (_value); break; }
    }
}