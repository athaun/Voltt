#pragma once

#include "parser.hpp"

namespace Voltt {
namespace ASTGen {

auto ast_dump_node_indent(std::ostream& _os, const size_t _level) -> void;
auto ast_dump_node(std::ostream&, const ASTNode::Node*, const size_t = 0) -> void;

} // namespace ASTGen
} // namespace Voltt
