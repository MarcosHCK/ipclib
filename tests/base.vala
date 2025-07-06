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

namespace Testing
{
  public const string TESTPATHROOT = "/org/hck/IpcLib";

  public abstract class BaseTestCase : GLib.Object
    {

      public static GLib.Bytes rand_bytes (size_t sz)
        {
          var ar = new uint8 [sz];
          var begin = (int32) uint8.MIN;
          var end = (int32) uint8.MAX;

          for (size_t i = 0; i < sz; ++i) ar [i] = (uint8) GLib.Test.rand_int_range (begin, end);
          return new GLib.Bytes.take ((owned) ar);
        }

      public static int rand_enum<T> (GLib.Type gtype = typeof (T)) requires (gtype.is_enum ())
        {
          int32 begin = 0, end = 0;
          var klass = (EnumClass) gtype.class_ref ();

          foreach (unowned var value in klass.values) if (value.value > end) end = value.value;
          return GLib.Test.rand_int_range (begin, end + 1);
        }

      public static uint rand_flags<T> (GLib.Type gtype = typeof (T)) requires (gtype.is_flags ())
        {
          var acc = (uint) 0;
          var mask = (uint) 0;
          var klass = (FlagsClass) gtype.class_ref ();
          var used = (int) 0;

          foreach (unowned var value in klass.values)
            {
              if (used % (sizeof (uint) << 3) == 0) mask = (uint) GLib.Test.rand_int ();
              if (1 == (mask & 1)) acc |= value.value;
              mask >>= 1; ++used;
            }
          return acc;
        }

      public static string rand_string (ssize_t _length = -1)
        {
          var length = 0 < _length ? _length : GLib.Test.rand_int_range (0, 20);
          var str = new GLib.StringBuilder.sized (length);

          for (size_t i = 0; i < length; ++i)
            {
              var c = GLib.Test.rand_int_range (0x21, 0x7f);
              str.append_c ((char) c);
            }
          return str.free_and_steal ();
        }

      public static unowned T rand_pick<T> (T[] from)
        {
          var pick = GLib.Test.rand_int_range (0, from.length);
        return from [pick];
        }

      public static GLib.Variant rand_variant (bool simple = false)
        {
          var signature = _rand_signature (simple);
          var variant = _rand_variant (signature, simple);
        return variant;
        }

      [CCode (cheader_filename = "base.h", cname = "_testing_rand_object_path")]
      static extern string _rand_object_path ();

      [CCode (cheader_filename = "base.h", cname = "_testing_rand_signature")]
      static extern GLib.VariantType _rand_signature (bool simple, bool strict = false);

      static GLib.Variant _rand_variant (GLib.VariantType signature, bool simple)
        {

          if (signature.is_array ()) return _rand_variant_array (signature, simple);
          else if (signature.is_basic ()) return _rand_variant_simple (signature, simple);
          else if (signature.is_dict_entry ()) return _rand_variant_dict_entry (signature, simple);
          else if (signature.is_maybe ()) return _rand_variant_maybe (signature, simple);
          else if (signature.is_tuple ()) return _rand_variant_tuple (signature, simple);
          else if (signature.is_variant ()) return _rand_variant_simple (signature, simple);
          else assert_not_reached ();
        }

      static GLib.Variant _rand_variant_array (GLib.VariantType signature, bool simple)
        {
          unowned var size = GLib.Test.rand_int_range (0, 100);
          unowned var type = signature.element ();

          var array = new GenericArray<GLib.Variant> (size);

          for (uint i = 0; i < (uint) size; ++i)
            {
              var inner = _rand_variant (type, simple);
              array.add ((owned) inner);
            }
        return new GLib.Variant.array (type, array.steal ());
        }

      static GLib.Variant _rand_variant_dict_entry (GLib.VariantType signature, bool simple)
        {
          unowned var ctype1 = signature.first ();
          unowned var ctype2 = ctype1.next ();

          var inner1 = _rand_variant (ctype1, simple);
          var inner2 = _rand_variant (ctype2, simple);
        return new GLib.Variant.dict_entry (inner1, inner2);
        }

      static GLib.Variant _rand_variant_maybe (GLib.VariantType signature, bool simple)
        {
          unowned var have = GLib.Test.rand_bit ();
          unowned var type = signature.element ();

          var inner = ! have ? null : _rand_variant (type, simple);
        return new GLib.Variant.maybe (type, inner);
        }

      static GLib.Variant _rand_variant_simple (GLib.VariantType signature, bool simple)
        {

          switch (signature.peek_string () [0])
            {

              /* simple types */
              case 'b': { var value = GLib.Test.rand_bit ();
                          return new GLib.Variant.boolean (value); }
              case 'd': { var value = GLib.Test.rand_double ();
                          return new GLib.Variant.double (value); }
              case 'g': { var value = _rand_signature (false, true);
                          return new GLib.Variant.signature ((string) value.peek_string ()); }
              case 'h': { var value = GLib.Test.rand_double_range (int32.MIN, int32.MAX);
                          return new GLib.Variant.handle ((int32) value); }
              case 'i': { var value = GLib.Test.rand_double_range (int32.MIN, int32.MAX);
                          return new GLib.Variant.int32 ((int32) value); }
              case 'n': { var value = GLib.Test.rand_double_range (int16.MIN, int16.MAX);
                          return new GLib.Variant.int16 ((int16) value); }
              case 'o': { var value = _rand_object_path ();
                          return new GLib.Variant.object_path (value); }
              case 'q': { var value = GLib.Test.rand_double_range (uint16.MIN, uint16.MAX);
                          return new GLib.Variant.uint16 ((uint16) value); }
              case 's': { var value = rand_string (GLib.Test.rand_int_range (0, 100));
                          return new GLib.Variant.string (value); }
              case 't': { var value = GLib.Test.rand_double_range (uint64.MIN, uint64.MAX);
                          return new GLib.Variant.uint64 ((uint64) value); }
              case 'u': { var value = GLib.Test.rand_double_range (uint32.MIN, uint32.MAX);
                          return new GLib.Variant.uint32 ((uint32) value); }
              case 'v': { var value = rand_variant (simple);
                          return new GLib.Variant.variant (value); }
              case 'x': { var value = GLib.Test.rand_double_range (int64.MIN, int64.MAX);
                          return new GLib.Variant.int64 ((int64) value); }
              case 'y': { var value = GLib.Test.rand_double_range (uint8.MIN, uint8.MAX);
                          return new GLib.Variant.byte ((uint8) value); }

              default: assert_not_reached ();
            }
        }

      static GLib.Variant _rand_variant_tuple (GLib.VariantType signature, bool simple)
        {

          var length = signature.n_items ();
          var array = new GenericArray<GLib.Variant> ((uint) length);

          for (unowned GLib.VariantType ctype = signature.first (); ctype != null; ctype = ctype.next ())
            {
              var inner = _rand_variant (ctype, simple);
              array.add ((owned) inner);
            }
        return new GLib.Variant.tuple (array.steal ());
        }

      public abstract void run (GLib.MainContext? context = null);
    }

  public abstract class AsyncTest : BaseTestCase
    {

      public override void run (GLib.MainContext? context = null)
        {
          var loop = new GLib.MainLoop (context, false);

          test.begin ((o, res) =>
            {
              test.end (res);
              loop.quit ();
            });

          loop.run ();
        }

      public void run_in_thread ()
        {
          MainContext context;

          (context = new GLib.MainContext ()).push_thread_default ();
          run (context);
          context.pop_thread_default ();
        }

      protected abstract async void test ();

      public static void wait (uint[] dones)
        {
          var alpha = dones.length;
          var context = GLib.MainContext.get_thread_default ();
          var loop = new GLib.MainLoop (context, true);
          var source = new GLib.IdleSource ();

          source.set_callback (() =>
            {
              uint pending = 0;

              for (unowned var i = 0; i < alpha; ++i)
                {
                  pending |= AtomicUint.get (ref dones [i]) ^ 1;
                }

              if (pending == 0)
                {
                  loop.quit ();
                  return GLib.Source.REMOVE;
                }

              return GLib.Source.CONTINUE;
            });

          source.set_priority (GLib.Priority.HIGH_IDLE);
          source.attach (context);
          loop.run ();
        }
    }

  public abstract class SyncTest : BaseTestCase
    {

      public override void run (GLib.MainContext? context = null)
        {
          test ();
        }

      protected abstract void test ();
    }

  [CCode (scope = "notified")]
  public delegate string ToString<T> (T item);

  public static string serialize_array<T> (T[] ar, owned ToString<T> func)
    {
      var builder = new StringBuilder ("[ ");
      var first = true;

      foreach (unowned var item in ar)
        {
          if (first)

            first = false;
          else
            builder.append (", ");
            builder.append (func (item));
        }
      builder.append (" ]");
      return builder.free_and_steal ();
    }

  public static string serialize_list<T> (GLib.List<T> list, owned ToString<T> func)
    {
      var builder = new StringBuilder ("[ ");
      var first = true;

      foreach (unowned var item in list)
        {
          if (first)

            first = false;
          else
            builder.append (", ");
            builder.append (func (item));
        }
      builder.append (" ]");
      return builder.free_and_steal ();
    }

  public static string serialize_slist<T> (GLib.SList<T> list, owned ToString<T> func)
    {
      var builder = new StringBuilder ("[ ");
      var first = true;

      foreach (unowned var item in list)
        {
          if (first)

            first = false;
          else
            builder.append (", ");
            builder.append (func (item));
        }
      builder.append (" ]");
      return builder.free_and_steal ();
    }
}
