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
#include <report.h>

void _g_task_report_boolean (gpointer source_object, GAsyncReadyCallback callback, gpointer callback_data, gpointer source_tag, gboolean result)
{
  GTask* task = g_task_new (source_object, NULL, callback, callback_data);

  (g_task_set_source_tag) (task, source_tag);
  (g_task_set_static_name) (task, G_STRFUNC);
  g_task_return_boolean (task, result);
  g_object_unref (task);
}

void _g_task_report_pointer (gpointer source_object, GAsyncReadyCallback callback, gpointer callback_data, gpointer source_tag, gpointer result, GDestroyNotify result_notify)
{
  GTask* task = g_task_new (source_object, NULL, callback, callback_data);

  (g_task_set_source_tag) (task, source_tag);
  (g_task_set_static_name) (task, G_STRFUNC);
  g_task_return_pointer (task, result, result_notify);
  g_object_unref (task);
}