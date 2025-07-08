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

[CCode (cheader_filename = "ipclib.h", cprefix = "Ipc", lower_case_cprefix = "ipc_")]

namespace Ipc
{

  [CCode (cheader_filename = "proxyhandler.h")]

  public errordomain ProxyHandlerError
    {
      INVALID,
      UNKNOWN_CONVERSION,
      UNKNOWN_METHOD,
      UNKNOWN_PROPERTY;
      public static GLib.Quark quark ();
    }

  [CCode (cheader_filename = "proxyhandler.h")]

  namespace ProxyHandler
    {
      [CCode (returns_floating_reference = true)]
      public static GLib.Variant get_property_pack (string property_name);
      [CCode (returns_floating_reference = true)]
      public static GLib.Variant set_property_pack (string property_name, GLib.Variant variant);
    }

  [CCode (cheader_filename = "proxyhandler.h")]

  public class DBusProxyHandler : GLib.Object, Ipc.Handler
    {
      public GLib.DBusProxy proxiee { get; construct; }
      public int timeout { get; construct set; }

      [CCode (has_construct_function = false)]
      public DBusProxyHandler (GLib.DBusProxy proxiee);
    }

  [CCode (cheader_filename = "proxyhandler.h")]

  public class GObjectProxyHandler : GLib.Object, Ipc.Handler
    {
      public GLib.Object proxiee { get; construct; }

      [CCode (has_construct_function = false)]
      public GObjectProxyHandler (GLib.Object proxiee);
      [CCode (returns_floating_reference = false)]
      public virtual async GLib.Variant invoke (string method, GLib.Variant @params, GLib.Cancellable? cancellable = null) throws GLib.Error;
    }
}