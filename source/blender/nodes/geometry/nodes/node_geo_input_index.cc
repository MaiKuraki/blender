/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

#include "node_geometry_util.hh"

namespace blender::nodes::node_geo_input_index_cc {

static void node_declare(NodeDeclarationBuilder &b)
{
  b.add_output<decl::Int>("Index").field_source();
}

static void node_geo_exec(GeoNodeExecParams params)
{
  Field<int> index_field{std::make_shared<fn::IndexFieldInput>()};
  params.set_output("Index", std::move(index_field));
}

static void node_register()
{
  static blender::bke::bNodeType ntype;

  geo_node_type_base(&ntype, "GeometryNodeInputIndex", GEO_NODE_INPUT_INDEX, NODE_CLASS_INPUT);
  ntype.ui_name = "Index";
  ntype.ui_description =
      "Retrieve an integer value indicating the position of each element in the list, starting at "
      "zero";
  ntype.enum_name_legacy = "INDEX";
  ntype.geometry_node_execute = node_geo_exec;
  ntype.declare = node_declare;
  blender::bke::node_register_type(&ntype);
}
NOD_REGISTER_NODE(node_register)

}  // namespace blender::nodes::node_geo_input_index_cc
