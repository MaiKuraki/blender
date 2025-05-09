/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

#include "gpu_shader_create_info.hh"

GPU_SHADER_CREATE_INFO(compositor_split_shared)
LOCAL_GROUP_SIZE(16, 16)
PUSH_CONSTANT(float, split_ratio)
SAMPLER(0, sampler2D, first_image_tx)
SAMPLER(1, sampler2D, second_image_tx)
IMAGE(0, GPU_RGBA16F, write, image2D, output_img)
COMPUTE_SOURCE("compositor_split.glsl")
GPU_SHADER_CREATE_END()

GPU_SHADER_CREATE_INFO(compositor_split_horizontal)
ADDITIONAL_INFO(compositor_split_shared)
DEFINE("SPLIT_HORIZONTAL")
DO_STATIC_COMPILATION()
GPU_SHADER_CREATE_END()

GPU_SHADER_CREATE_INFO(compositor_split_vertical)
ADDITIONAL_INFO(compositor_split_shared)
DEFINE("SPLIT_VERTICAL")
DO_STATIC_COMPILATION()
GPU_SHADER_CREATE_END()
