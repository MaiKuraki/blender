# SPDX-FileCopyrightText: 2020 Blender Authors
#
# SPDX-License-Identifier: GPL-2.0-or-later

if(WITH_IO_WAVEFRONT_OBJ OR
   WITH_IO_PLY OR
   WITH_IO_STL OR
   WITH_IO_GREASE_PENCIL OR
   WITH_ALEMBIC OR
   WITH_USD OR
   WITH_IO_CSV OR
   WITH_IO_FBX)
  add_subdirectory(common)
endif()

if(WITH_IO_WAVEFRONT_OBJ)
  add_subdirectory(wavefront_obj)
endif()

if(WITH_IO_PLY)
  add_subdirectory(ply)
endif()

if(WITH_IO_STL)
  add_subdirectory(stl)
endif()

if(WITH_IO_GREASE_PENCIL)
  add_subdirectory(grease_pencil)
endif()

if(WITH_IO_FBX)
  add_subdirectory(fbx)
endif()

if(WITH_ALEMBIC)
  add_subdirectory(alembic)
endif()

if(WITH_OPENCOLLADA)
  add_subdirectory(collada)
endif()

if(WITH_USD)
  add_subdirectory(usd)
endif()

if(WITH_IO_CSV)
  add_subdirectory(csv)
endif()
