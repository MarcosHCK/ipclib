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

  static GLib.Variant rand_tuple ()
    {

      var length = GLib.Test.rand_int_range (1, 10);
      var array = new GenericArray<GLib.Variant> (length);

      for (uint i = 0; i < (uint) length; ++i)
        {
          var variant = BaseTestCase.rand_variant ();
          array.add ((owned) variant);
        }
    return new GLib.Variant.tuple (array.steal ());
    }

  class CoreCallTest : SyncTest
    {

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

  class ClassEndpointTest : AsyncTest
    {

      private Ipc.Endpoint endpoint;
      private DummyHandler[] handlers;

      construct
        {
          uint length;
          var param_type = (GLib.VariantType) Ipc.CALL_SIGNATURE;

          length = (uint) GLib.Test.rand_int_range (1, 10);
          handlers = new DummyHandler [length];

          for (uint i = 0; i < length; ++i)
            {
              var id = rand_string ();
              endpoint.add_handler (handlers [i] = new DummyHandler (id), id, param_type);
            }
        }

      protected override async void test ()
        {
          yield test_error ();
          yield test_normal ();
        }

      private async void test_error ()
        {
          var id = rand_string ();
          var endpoint = new Ipc.Endpoint ();
          var handler = new ErrorHandler ();
          var param_type = (GLib.VariantType) Ipc.CALL_SIGNATURE;

          endpoint.add_handler (handler, id, param_type);

          var name = rand_string ();
          var params = rand_tuple ();

          var call = Ipc.call_pack (name, params);

          try { yield endpoint.handle (id, call); } catch (GLib.Error e)
            {
              unowned var code = IOError.FAILED;
              unowned var domain = IOError.quark ();

              assert_error (e, domain, code);
            }
        }

      private async void test_normal ()
        {

          var handler = rand_pick (handlers);
          var id1 = handler.id;

          var name1 = rand_string ();
          var params1 = rand_variant ();

          var call = Ipc.call_pack (name1, params1);

          GLib.Variant result, reply;

          try { result = yield endpoint.handle (id1, call); } catch (GLib.Error e)
            {
              assert_no_error (e);
              return;
            }

          try { reply = Ipc.reply_unpack (result); } catch (GLib.Error e)
            {
              assert_no_error (e);
              return;
            }

          var id2 = reply.get_child_value (0).get_string ();
          var name2 = reply.get_child_value (1).get_string ();
          var params2 = reply.get_child_value (2);

          assert_cmpstr (id1, GLib.CompareOperator.EQ, id2);
          assert_cmpstr (name1, GLib.CompareOperator.EQ, name2);
          assert_cmpvariant (params1, params2);
        }

      class ErrorHandler : GLib.Object, Ipc.Handler
        {

          public async GLib.Variant handle (GLib.Variant call, GLib.Cancellable? cancellable) throws GLib.Error
            {
              throw new IOError.FAILED ("testing");
            }
        }

      class DummyHandler : GLib.Object, Ipc.Handler
        {

          public string id { get; construct; }

          public DummyHandler (string id)
            {
              Object (id: id);
            }

          public async GLib.Variant handle (GLib.Variant call, GLib.Cancellable? cancellable) throws GLib.Error
            {
              GLib.Variant params;
              string name = Ipc.call_unpack (call, out params);
              var _id = new GLib.Variant.string (id);
              var _name = new GLib.Variant.string (name);
            return Ipc.reply_pack (new GLib.Variant.tuple ({ _id, _name, params }));
            }
        }
    }

  class CoreHandlerTest : AsyncTest
    {

      protected override async void test ()
        {
          yield test_error ();
          yield test_normal ();
        }

      private async void test_error ()
        {
          var name = rand_string ();
          var params = rand_tuple ();

          var call = Ipc.call_pack (name, params);
          var handler = new ErrorHandler ();

          try { yield handler.handle (call); } catch (GLib.Error e)
            {
              unowned var code = IOError.FAILED;
              unowned var domain = IOError.quark ();

              assert_error (e, domain, code);
            }
        }

      private async void test_normal ()
        {
          var name1 = rand_string ();
          var params1 = rand_tuple ();

          var call = Ipc.call_pack (name1, params1);
          var handler = new DummyHandler ();

          GLib.Variant result, reply;

          try { result = yield handler.handle (call); } catch (GLib.Error e)
            {
              assert_no_error (e);
              return;
            }

          try { reply = Ipc.reply_unpack (result); } catch (GLib.Error e)
            {
              assert_no_error (e);
              return;
            }

          var name2 = reply.get_child_value (0).get_string ();
          var params2 = reply.get_child_value (1);

          assert_cmpstr (name1, GLib.CompareOperator.EQ, name2);
          assert_cmpvariant (params1, params2);
        }

      class ErrorHandler : GLib.Object, Ipc.Handler
        {

          public async GLib.Variant handle (GLib.Variant call, GLib.Cancellable? cancellable) throws GLib.Error
            {
              throw new IOError.FAILED ("testing");
            }
        }

      class DummyHandler : GLib.Object, Ipc.Handler
        {

          public async GLib.Variant handle (GLib.Variant call, GLib.Cancellable? cancellable) throws GLib.Error
            {
              GLib.Variant params;
              string name = Ipc.call_unpack (call, out params);
            return Ipc.reply_pack (new GLib.Variant.tuple ({ new GLib.Variant.string (name), params }));
            }
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
      GLib.Test.add_func (TESTPATHROOT + "/core/handler", () => (new CoreHandlerTest ()).run ());
      GLib.Test.add_func (TESTPATHROOT + "/core/replay", () => (new CoreReplayTest ()).run ());
    return GLib.Test.run ();
    }
}