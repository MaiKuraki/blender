/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup gpu
 */

#include "vk_state_manager.hh"
#include "vk_context.hh"
#include "vk_index_buffer.hh"
#include "vk_shader.hh"
#include "vk_storage_buffer.hh"
#include "vk_texture.hh"
#include "vk_vertex_buffer.hh"

#include "GPU_capabilities.hh"

namespace blender::gpu {

void VKStateManager::apply_state()
{
  /* Intentionally empty. State is polled during pipeline creation and doesn't need to be applied.
   * If this leads to issues we should have an active state. */
}

void VKStateManager::apply_bindings(VKContext &context,
                                    render_graph::VKResourceAccessInfo &resource_access_info)
{
  VKShader *shader = unwrap(context.shader);
  if (shader == nullptr) {
    return;
  }
  AddToDescriptorSetContext data(
      context.descriptor_set_get(), shader->interface_get(), resource_access_info);
  textures_.add_to_descriptor_set(data);
  images_.add_to_descriptor_set(data);
  uniform_buffers_.add_to_descriptor_set(data);
  storage_buffers_.add_to_descriptor_set(data);
}

void VKStateManager::force_state()
{
  /* Intentionally empty. State is polled during pipeline creation and is always forced. */
}

void VKStateManager::issue_barrier(eGPUBarrier barrier_bits)
{
  /**
   * Workaround for EEVEE ThicknessFromShadow shader.
   *
   * EEVEE light evaluation uses layered sub-pass tracking. Currently, the tracking supports
   * transitioning a layer to a different layout once per rendering scope. When using the thickness
   * from shadow, the layers need to be transitioned twice: once to image load/store for the
   * thickness from shadow shader and then to a sampler for the light evaluation shader. We work
   * around this limitation by suspending the rendering.
   *
   * The reason we need to suspend the rendering is that Vulkan, by default, doesn't support layout
   * transitions between the begin and end of rendering. By suspending the render, the graph will
   * create a new node group that allows the necessary image layout transition.
   *
   * This limitation could also be addressed in the render graph scheduler, but that would be quite
   * a hassle to track and might not be worth the effort.
   */
  if (bool(barrier_bits & GPU_BARRIER_SHADER_IMAGE_ACCESS)) {
    VKContext &context = *VKContext::get();
    context.rendering_end();
  }
}

void VKStateManager::texture_bind(Texture *tex, GPUSamplerState sampler, int unit)
{
  VKTexture *texture = unwrap(tex);
  textures_.bind(unit, *texture, sampler);
}

void VKStateManager::texture_unbind(Texture *tex)
{
  VKTexture *texture = unwrap(tex);
  textures_.unbind(*texture);
}

void VKStateManager::texture_unbind_all()
{
  textures_.unbind_all();
}

void VKStateManager::image_bind(Texture *tex, int binding)
{
  VKTexture *texture = unwrap(tex);
  images_.bind(binding, *texture);
}

void VKStateManager::image_unbind(Texture *tex)
{
  VKTexture *texture = unwrap(tex);
  images_.unbind(*texture);
}

void VKStateManager::image_unbind_all()
{
  images_.unbind_all();
}

void VKStateManager::uniform_buffer_bind(VKUniformBuffer *uniform_buffer, int slot)
{
  uniform_buffers_.bind(slot, *uniform_buffer);
}

void VKStateManager::uniform_buffer_unbind(VKUniformBuffer *uniform_buffer)
{
  uniform_buffers_.unbind(*uniform_buffer);
}

void VKStateManager::uniform_buffer_unbind_all()
{
  uniform_buffers_.unbind_all();
}

void VKStateManager::unbind_from_all_namespaces(VKBindableResource &resource)
{
  uniform_buffers_.unbind(resource);
  storage_buffers_.unbind(resource);
  images_.unbind(resource);
  textures_.unbind(resource);
}

void VKStateManager::texel_buffer_bind(VKVertexBuffer &vertex_buffer, int slot)
{
  textures_.bind(slot, vertex_buffer);
}

void VKStateManager::texel_buffer_unbind(VKVertexBuffer &vertex_buffer)
{
  textures_.unbind(vertex_buffer);
}

void VKStateManager::storage_buffer_bind(VKBindableResource &resource, int slot)
{
  storage_buffers_.bind(slot, resource);
}

void VKStateManager::storage_buffer_unbind(VKBindableResource &resource)
{
  storage_buffers_.unbind(resource);
}

void VKStateManager::storage_buffer_unbind_all()
{
  storage_buffers_.unbind_all();
}

void VKStateManager::texture_unpack_row_length_set(uint len)
{
  texture_unpack_row_length_ = len;
}

uint VKStateManager::texture_unpack_row_length_get() const
{
  return texture_unpack_row_length_;
}

}  // namespace blender::gpu
