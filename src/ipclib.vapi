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

  public const string CALL_SIGNATURE;

  [CCode (returns_floating_reference = true)]
  public static GLib.Variant call_pack (string name, GLib.Variant @params);
  public static string call_unpack (GLib.Variant call, [CCode (returns_floating_reference = false)] out GLib.Variant @params);
  [CCode (returns_floating_reference = true)]
  public static GLib.Variant reply_pack (GLib.Variant? reply, GLib.Error? error = null);
  [CCode (returns_floating_reference = false)]
  public static GLib.Variant reply_unpack (GLib.Variant reply) throws GLib.Error;

  public class Endpoint : GLib.Object
    {
      public Endpoint ();
      public void add_handler (Ipc.Handler handler, string name, GLib.VariantType param_type);
      public void delete_handler (string name);
      [HasEmitter]
      public signal void emit (string name, GLib.Variant @params);
      [CCode (returns_floating_reference = false)]
      public async GLib.Variant handle (string name, GLib.Variant @params, GLib.Cancellable? cancellable = null) throws GLib.Error;
    }

  public interface Handler : GLib.Object
    {
      [HasEmitter]
      public abstract signal void emit (GLib.Variant @params);
      [CCode (returns_floating_reference = false)]
      public abstract async GLib.Variant handle (GLib.Variant @params, GLib.Cancellable? cancellable = null) throws GLib.Error;
    }
}
