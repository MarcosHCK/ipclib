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

      public static string rand_string (size_t length)
        {
          var str = new GLib.StringBuilder.sized (length);

          for (size_t i = 0; i < length; ++i)
            {
              var c = GLib.Test.rand_int_range (0x21, 0x7f);
              str.append_c ((char) c);
            }
          return str.free_and_steal ();
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
