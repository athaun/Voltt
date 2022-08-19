#pragma once

#include "../frontend/astnode.hpp"

#include <llvm/IR/Value.h>

namespace Voltt {
namespace IRVisitor {

auto ir_gen(ASTNode::Node* _node) -> llvm::Value*;
auto ir_variable_gen(ASTNode::Node* _node) -> llvm::Value*;
auto ir_literal_gen(ASTNode::Node* _node) -> llvm::Value*;

} // namespace IRVisitor
} // namespace Voltt
