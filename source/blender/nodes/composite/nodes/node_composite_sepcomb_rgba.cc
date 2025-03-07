/* SPDX-FileCopyrightText: 2006 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup cmpnodes
 */

#include "GPU_material.hh"

#include "COM_shader_node.hh"

#include "node_composite_util.hh"

/* **************** SEPARATE RGBA ******************** */

namespace blender::nodes::node_composite_separate_rgba_cc {

static void cmp_node_seprgba_declare(NodeDeclarationBuilder &b)
{
  b.add_input<decl::Color>("Image")
      .default_value({1.0f, 1.0f, 1.0f, 1.0f})
      .compositor_domain_priority(0);
  b.add_output<decl::Float>("R").translation_context(BLT_I18NCONTEXT_COLOR);
  b.add_output<decl::Float>("G").translation_context(BLT_I18NCONTEXT_COLOR);
  b.add_output<decl::Float>("B").translation_context(BLT_I18NCONTEXT_COLOR);
  b.add_output<decl::Float>("A").translation_context(BLT_I18NCONTEXT_COLOR);
}

using namespace blender::compositor;

class SeparateRGBAShaderNode : public ShaderNode {
 public:
  using ShaderNode::ShaderNode;

  void compile(GPUMaterial *material) override
  {
    GPUNodeStack *inputs = get_inputs_array();
    GPUNodeStack *outputs = get_outputs_array();

    GPU_stack_link(material, &bnode(), "node_composite_separate_rgba", inputs, outputs);
  }
};

static ShaderNode *get_compositor_shader_node(DNode node)
{
  return new SeparateRGBAShaderNode(node);
}

}  // namespace blender::nodes::node_composite_separate_rgba_cc

void register_node_type_cmp_seprgba()
{
  namespace file_ns = blender::nodes::node_composite_separate_rgba_cc;

  static blender::bke::bNodeType ntype;

  cmp_node_type_base(
      &ntype, "CompositorNodeSepRGBA", CMP_NODE_SEPRGBA_LEGACY, NODE_CLASS_CONVERTER);
  ntype.ui_name = "Separate RGBA (Legacy)";
  ntype.ui_description = "Deprecated";
  ntype.enum_name_legacy = "SEPRGBA";
  ntype.declare = file_ns::cmp_node_seprgba_declare;
  ntype.gather_link_search_ops = nullptr;
  ntype.get_compositor_shader_node = file_ns::get_compositor_shader_node;

  blender::bke::node_register_type(&ntype);
}

/* **************** COMBINE RGBA ******************** */

namespace blender::nodes::node_composite_combine_rgba_cc {

static void cmp_node_combrgba_declare(NodeDeclarationBuilder &b)
{
  b.add_input<decl::Float>("R")
      .min(0.0f)
      .max(1.0f)
      .compositor_domain_priority(0)
      .translation_context(BLT_I18NCONTEXT_COLOR);
  b.add_input<decl::Float>("G")
      .min(0.0f)
      .max(1.0f)
      .compositor_domain_priority(1)
      .translation_context(BLT_I18NCONTEXT_COLOR);
  b.add_input<decl::Float>("B")
      .min(0.0f)
      .max(1.0f)
      .compositor_domain_priority(2)
      .translation_context(BLT_I18NCONTEXT_COLOR);
  b.add_input<decl::Float>("A")
      .default_value(1.0f)
      .min(0.0f)
      .max(1.0f)
      .compositor_domain_priority(3)
      .translation_context(BLT_I18NCONTEXT_COLOR);
  b.add_output<decl::Color>("Image");
}

using namespace blender::compositor;

class CombineRGBAShaderNode : public ShaderNode {
 public:
  using ShaderNode::ShaderNode;

  void compile(GPUMaterial *material) override
  {
    GPUNodeStack *inputs = get_inputs_array();
    GPUNodeStack *outputs = get_outputs_array();

    GPU_stack_link(material, &bnode(), "node_composite_combine_rgba", inputs, outputs);
  }
};

static ShaderNode *get_compositor_shader_node(DNode node)
{
  return new CombineRGBAShaderNode(node);
}

}  // namespace blender::nodes::node_composite_combine_rgba_cc

void register_node_type_cmp_combrgba()
{
  namespace file_ns = blender::nodes::node_composite_combine_rgba_cc;

  static blender::bke::bNodeType ntype;

  cmp_node_type_base(
      &ntype, "CompositorNodeCombRGBA", CMP_NODE_COMBRGBA_LEGACY, NODE_CLASS_CONVERTER);
  ntype.ui_name = "Combine RGBA (Legacy)";
  ntype.ui_description = "Deprecated";
  ntype.enum_name_legacy = "COMBRGBA";
  ntype.declare = file_ns::cmp_node_combrgba_declare;
  ntype.gather_link_search_ops = nullptr;
  ntype.get_compositor_shader_node = file_ns::get_compositor_shader_node;

  blender::bke::node_register_type(&ntype);
}
