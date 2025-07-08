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

  [DBus (name = "org.hck.IpcLib.Interface")]

  interface Interface : GLib.Object
    {

      [DBus (name = "Quit")] public abstract void quit () throws GLib.Error;
      [DBus (name = "TestMethod1")] public abstract string test_method_1 (string arg) throws GLib.Error;
      [DBus (name = "TestMethod2")] public abstract uint test_method_2 (string arg) throws GLib.Error;
    }

  class InterfaceImpl : GLib.Object, Interface
    {

      public GLib.MainLoop loop { get; construct; }

      public InterfaceImpl (GLib.MainLoop loop)
        {
          Object (loop: loop);
        }

      public void quit () throws GLib.Error
        {
          loop.quit ();
        }

      public string test_method_1 (string arg) throws GLib.Error
        {
          return arg;
        }

      public uint test_method_2 (string arg) throws GLib.Error
        {
          return GLib.str_hash (arg);
        }
    }

  [CCode (cheader_filename = "glib.h", cname = "G_USEC_PER_SEC")]
  extern const uint _G_USEC_PER_SEC;

  class ProxyDBus : SyncTest
    {

      protected override void test ()
        {

          if (GLib.Test.subprocess ())

            test_child ();
          else
            { test_parent_setup ();
              GLib.Test.trap_subprocess (null, 0, 0);
              GLib.Test.trap_assert_passed (); }
        }

      private void on_name_acquired (GLib.DBusConnection connection, GLib.MainLoop loop)
        {

          var object = new InterfaceImpl (loop);
          var object_path = "/org/hck/IpcLib";

          try { connection.register_object<Interface> (object_path, object); } catch (GLib.Error e)
            {
              assert_no_error (e);
            }
        }

      private void on_name_lost (GLib.DBusConnection? connection, bool acquired)
        {
          if (connection == null)
            GLib.Test.fail_printf ("can not acquire the bus");
          if (acquired == false)
            GLib.Test.fail_printf ("can not acquire the dbus name");
        }

      private void test_child ()
        {
          var bus_type = GLib.BusType.SESSION;
          var flags = GLib.BusNameOwnerFlags.NONE;
          var loop = new MainLoop (null);
          var name = "org.hck.IpcLib";

          bool acquired = false;
          uint owner_id = GLib.Bus.own_name (bus_type, name, flags, null,
            c => { acquired = true; on_name_acquired (c, loop); },
            c => { on_name_lost (c, acquired); });

          loop.run ();
          GLib.Bus.unown_name (owner_id);
        }

      private async void test_parent (Interface proxiee)
        {
          string value1 = rand_string ();
          string value2;

          try { value2 = proxiee.test_method_1 (value1); } catch (GLib.Error e)
            {
              assert_no_error (e);
              return;
            }

          /* sanity check before real work */
          assert_cmpstr (value1, GLib.CompareOperator.EQ, value2);

          var proxy = new Ipc.DBusProxyHandler ((GLib.DBusProxy) proxiee);
          var params = Ipc.call_pack ("TestMethod1", new GLib.Variant ("(s)", value1));

          try { value2 = (yield proxy.handle (params)).get_child_value (0).get_string (); } catch (GLib.Error e)
            {
              assert_no_error (e);
              return;
            }

          assert_cmpstr (value1, GLib.CompareOperator.EQ, value2);

          uint hash1 = GLib.str_hash (value1);
          uint hash2;

          params = Ipc.call_pack ("TestMethod2", new GLib.Variant ("(s)", value1));

          try { hash2 = (yield proxy.handle (params)).get_child_value (0).get_uint32 (); } catch (GLib.Error e)
            {
              assert_no_error (e);
              return;
            }

          assert_cmpuint (hash1, GLib.CompareOperator.EQ, hash2);
        }

      static async void test_parent_acquire (GLib.DBusConnection connection, ProxyDBus self)
        {

          var flags = GLib.DBusProxyFlags.NONE;
          var name = "org.hck.IpcLib";
          var object_path = "/org/hck/IpcLib";
          Interface proxy;

          try { proxy = yield connection.get_proxy<Interface> (name, object_path, flags); } catch (GLib.Error e)
            {
              assert_no_error (e);
              return;
            }

          yield self.test_parent (proxy);

          try { proxy.quit (); } catch (GLib.Error e)
            {
              assert_no_error (e);
              return;
            }
        }

      static void test_parent_appeared (GLib.DBusConnection connection, ProxyDBus self, GLib.MainLoop loop)
        {

          GLib.Thread.usleep ((uint) (0.1 * (double) _G_USEC_PER_SEC));

          test_parent_acquire.begin (connection, self, (o, res) =>
            {
              test_parent_acquire.end (res);
              loop.quit ();
            });
        }

      static void test_parent_complete (GLib.Object? o, GLib.AsyncResult res)
        {
          try { ((GLib.Task) res).propagate_boolean (); } catch (GLib.Error e)
            { assert_no_error (e); }
        }

      private void test_parent_setup ()
        {
          unowned var callback = (GLib.AsyncReadyCallback) test_parent_complete;
          unowned var cancellable = (GLib.Cancellable?) null;
          unowned var source_object = (GLib.Object?) this;

          var task = new GLib.Task (source_object, cancellable, callback);

          task.run_in_thread ((task, s) => { test_parent_worker ((ProxyDBus) s);
                                                                             task.return_boolean (true); });
        }

      static void test_parent_worker (ProxyDBus self)
        {

          var bus_type = GLib.BusType.SESSION;
          var flags = GLib.BusNameWatcherFlags.NONE;
          var name = "org.hck.IpcLib";

          var context = GLib.MainContext.ref_thread_default ();
          var loop = new GLib.MainLoop (context);

          GLib.Bus.watch_name (bus_type, name, flags, c => test_parent_appeared (c, self, loop));
          loop.run ();
        }
    }

  class ProxyGObject : AsyncTest
    {

      class Dummy : GLib.Object
        {

          public int p_int { get; default = (int) GLib.Test.rand_int (); }
          public uint p_uint { get; default = (uint) GLib.Test.rand_int (); }
          public string p_string { get; set; default = rand_string (); }
        }

      class DummyProxy : Ipc.GObjectProxyHandler
        {

          public DummyProxy (GLib.Object proxiee)
            {
              Object (proxiee: proxiee);
            }

          public override async GLib.Variant invoke (string method, GLib.Variant @params, GLib.Cancellable? cancellable) throws GLib.Error
            {

              if (method == "test_method_1")

                return params;
              else
                return yield base.invoke (method, params, cancellable);
            }
        }

      protected override async void test ()
        {
          yield test_error ();
          yield test_normal ();
        }

      protected async void test_error ()
        {
          var proxiee = new Dummy ();
          var proxy = new Ipc.GObjectProxyHandler (proxiee);

          var params = Ipc.call_pack ("$get", new GLib.Variant ("(s)", "<miss>"));

          try { yield proxy.handle (params); } catch (GLib.Error e)
            {
              unowned var error_code = Ipc.ProxyHandlerError.UNKNOWN_PROPERTY;
              unowned var error_domain = Ipc.ProxyHandlerError.quark ();

              assert_error (e, error_domain, error_code);
              return;
            }

          params = Ipc.call_pack ("$set", new GLib.Variant ("(ss)", "<miss>", "<novalue>"));

          try { yield proxy.handle (params); } catch (GLib.Error e)
            {
              unowned var error_code = Ipc.ProxyHandlerError.UNKNOWN_PROPERTY;
              unowned var error_domain = Ipc.ProxyHandlerError.quark ();

              assert_error (e, error_domain, error_code);
              return;
            }

          params = Ipc.call_pack ("test_method_1", new GLib.Variant ("(s)", "<novalue>"));

          try { yield proxy.handle (params); } catch (GLib.Error e)
            {
              unowned var error_code = Ipc.ProxyHandlerError.UNKNOWN_METHOD;
              unowned var error_domain = Ipc.ProxyHandlerError.quark ();

              assert_error (e, error_domain, error_code);
              return;
            }
        }

      protected async void test_normal ()
        {
          var proxiee = new Dummy ();
          var proxy = new DummyProxy (proxiee);

          string value1 = proxiee.p_string;
          string value2;

          var params = Ipc.call_pack ("$get", new GLib.Variant ("(s)", "p-string"));

          try { value2 = (yield proxy.handle (params)).get_child_value (0).get_string (); } catch (GLib.Error e)
            {
              assert_no_error (e);
              return;
            }

          assert_cmpstr (value1, GLib.CompareOperator.EQ, value2);

          value1 = rand_string ();
          params = Ipc.call_pack ("$set", new GLib.Variant ("(ss)", "p-string", value1));
          bool good;

          try { good = (yield proxy.handle (params)).get_child_value (0).get_boolean (); } catch (GLib.Error e)
            {
              assert_no_error (e);
              return;
            }

          params = Ipc.call_pack ("$get", new GLib.Variant ("(s)", "p-string"));
          assert_true (good);

          try { value2 = (yield proxy.handle (params)).get_child_value (0).get_string (); } catch (GLib.Error e)
            {
              assert_no_error (e);
              return;
            }

          params = Ipc.call_pack ("test_method_1", new GLib.Variant ("(s)", value1));
          assert_cmpstr (value1, GLib.CompareOperator.EQ, value2);

          try { value2 = (yield proxy.handle (params)).get_child_value (0).get_string (); } catch (GLib.Error e)
            {
              assert_no_error (e);
              return;
            }

          assert_cmpstr (value1, GLib.CompareOperator.EQ, value2);
        }
    }

  public static int main (string[] args)
    {
      GLib.Test.init (ref args, null);
      GLib.Test.add_func (TESTPATHROOT + "/proxy/dbus", () => (new ProxyDBus ()).run ());
      GLib.Test.add_func (TESTPATHROOT + "/proxy/gobject", () => (new ProxyGObject ()).run ());
    return GLib.Test.run ();
    }
}