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
#ifndef __TESTING_BASE_IMPL_H__
#define __TESTING_BASE_IMPL_H__ 1
#include <glib.h>

#if __cplusplus
extern "C" {
#endif // __cplusplus

  G_GNUC_INTERNAL const gchar* _testing_rand_object_path (void);
  G_GNUC_INTERNAL GVariantType* _testing_rand_signature (gboolean simple, gboolean strict);

#if __cplusplus
}
#endif // __cplusplus

#endif // __TESTING_BASE_IMPL_H__