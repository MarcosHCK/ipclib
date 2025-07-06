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

  class CoreCallTest : SyncTest
    {

      public static GLib.Variant rand_tuple ()
        {

          var length = GLib.Test.rand_int_range (1, 10);
          var array = new GenericArray<GLib.Variant> (length);

          for (uint i = 0; i < (uint) length; ++i)
            {
              var variant = rand_variant ();
              array.add ((owned) variant);
            }
        return new GLib.Variant.tuple (array.steal ());
        }

      protected override void test ()
        {

          GLib.Variant params1 = rand_tuple ();
          GLib.Variant params2;

          var name1 = rand_string ();
          var call = Ipc.call_pack (name1, params1);

          var name2 = Ipc.call_unpack (call, out params2);

          assert_cmpstr (name1, GLib.CompareOperator.EQ, name2);
          assert_cmpvariant (params1, params2);
        }
    }

  class CoreReplayTest : SyncTest
    {

      protected override void test ()
        {
          test_error ();
          test_normal ();
        }

      private void test_error ()
        {

          var code1 = rand_enum<GLib.FileType> ();
          var domain1 = IOError.quark ();
          var message1 = rand_string ();

          var error1 = new GLib.Error.literal (domain1, code1, message1);
          var reply = Ipc.reply_pack (null, error1);

          try { Ipc.reply_unpack (reply); assert_not_reached (); } catch (GLib.Error error2)
            {
              unowned var code2 = error2.code;
              unowned var domain2 = error2.domain;
              unowned var message2 = error2.message;

              assert_cmpstr (domain1.to_string (), GLib.CompareOperator.EQ, domain2.to_string ());
              assert_cmpuint (code1, GLib.CompareOperator.EQ, code2);
              assert_cmpstr (message1, GLib.CompareOperator.EQ, message2);
            }
        }

      private void test_normal ()
        {
          GLib.Variant value1 = rand_variant ();
          GLib.Variant value2;

          var reply = Ipc.reply_pack (value1, null);

          try { value2 = Ipc.reply_unpack (reply); } catch (GLib.Error e)
            {
              assert_no_error (e);
              return;
            }

          assert_cmpvariant (value1, value2);
        }
    }

  public static int main (string[] args)
    {
      GLib.Test.init (ref args, null);
      GLib.Test.add_func (TESTPATHROOT + "/core/call", () => (new CoreCallTest ()).run ());
      GLib.Test.add_func (TESTPATHROOT + "/core/replay", () => (new CoreReplayTest ()).run ());
    return GLib.Test.run ();
    }
}