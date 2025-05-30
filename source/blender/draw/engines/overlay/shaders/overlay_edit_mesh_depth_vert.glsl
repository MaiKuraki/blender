/* SPDX-FileCopyrightText: 2017-2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

#include "infos/overlay_edit_mode_info.hh"

VERTEX_SHADER_CREATE_INFO(overlay_edit_mesh_depth)

#include "draw_model_lib.glsl"
#include "draw_view_clipping_lib.glsl"
#include "draw_view_lib.glsl"
#include "overlay_common_lib.glsl"

void main()
{
  float3 world_pos = drw_point_object_to_world(pos);
  float3 view_pos = drw_point_world_to_view(world_pos);
  gl_Position = drw_point_view_to_homogenous(view_pos);

  /* Offset Z position for retopology overlay. */
  gl_Position.z += get_homogenous_z_offset(
      drw_view().winmat, view_pos.z, gl_Position.w, retopology_offset);

  view_clipping_distances(world_pos);
}
