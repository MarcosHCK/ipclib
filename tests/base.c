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
#include "base.h"

const gchar* _testing_rand_object_path (void)
{
  gsize i, bits = g_test_rand_int_range (0, 20);
  GString* buffer = g_string_new (NULL);
  GPathBuf pathbuf = G_PATH_BUF_INIT;

  g_path_buf_push (&pathbuf, "/");

  static const gchar charset [] = "0123456789" "_"
                                  "abcdefghijklmnopqrstuvwxyz"
                                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static int charsetz = G_N_ELEMENTS (charset) - 1;

  for (i = 0; i < bits; ++i)
    {

      gsize j, length = g_test_rand_int_range (5, 15);

      g_string_set_size (buffer, length);

      for (j = 0; j < length; ++j)

        buffer->str [j] = charset [ g_test_rand_int_range (0, charsetz) ];

      g_path_buf_push (&pathbuf, buffer->str);
    }
return g_path_buf_clear_to_path (&pathbuf);
}

#define BASICS "bdghinoqstuxy"
#define COMPOUND "v"
#define CONTAINERS_DBUS "a({"
#define CONTAINERS "m" CONTAINERS_DBUS

enum
{
  NEXT_SIGNATURE_FLAG_IN_ARRAY = (1 << 0),
  NEXT_SIGNATURE_FLAG_SIMPLE = (1 << 1),
  NEXT_SIGNATURE_FLAG_STRICT = (1 << 2),
};

static GString* next_signature (GString* buffer, guint flags, guint depth)
{

  static const gchar any [] = BASICS COMPOUND CONTAINERS
                                              CONTAINERS;
  static const gchar any_dbus [] = BASICS COMPOUND CONTAINERS_DBUS
                                                   CONTAINERS_DBUS;
  static const gchar basics [] = BASICS;

  flags &= ~NEXT_SIGNATURE_FLAG_IN_ARRAY;
  flags |= depth < 6 ? 0 : NEXT_SIGNATURE_FLAG_SIMPLE;

  const gchar* from = (flags & NEXT_SIGNATURE_FLAG_SIMPLE)
                      ? basics : (! (flags & NEXT_SIGNATURE_FLAG_STRICT)
                                  ? any : any_dbus);
  const guint length = strlen (from);
  gchar pick;

  switch ((pick = from [ g_test_rand_int_range (0, length) ]))
    {

      case 'a':
        {
          flags |= NEXT_SIGNATURE_FLAG_IN_ARRAY;
          G_GNUC_FALLTHROUGH;
        }

      case 'm':
        {
          g_string_append_c (buffer, pick);

          buffer = next_signature (buffer, flags, 1 + depth);
          break;
        }

      case '(':
        {
          guint i;
          guint length = g_test_rand_int_range (1, 10);

          g_string_append_c (buffer, '(');

          for (i = 0; i < length; ++i)
            buffer = next_signature (buffer, flags, 1 + depth);

          g_string_append_c (buffer, ')');
          break;
        }

      case '{':
        {

          if ((flags & NEXT_SIGNATURE_FLAG_STRICT) && ! (flags & NEXT_SIGNATURE_FLAG_IN_ARRAY))
            {
              g_string_append_c (buffer, 'a');
            }

          g_string_append_c (buffer, '{');

          buffer = next_signature (buffer, flags | NEXT_SIGNATURE_FLAG_SIMPLE, 1 + depth);
          buffer = next_signature (buffer, flags, 1 + depth);
          g_string_append_c (buffer, '}');
          break;
        }

      default: g_string_append_c (buffer, pick);
        break;
    }
return buffer;
}

GVariantType* _testing_rand_signature (gboolean simple, gboolean strict)
{
  guint flags = 0
    | (! simple ? 0 : NEXT_SIGNATURE_FLAG_SIMPLE)
    | (! strict ? 0 : NEXT_SIGNATURE_FLAG_STRICT);
  GString* seed = g_string_new (NULL);
  GString* final = next_signature (seed, flags, 0);
return (GVariantType*) g_string_free_and_steal (final);
}