#pragma once

#include "tok.hpp"

#include <cstdint>

namespace Voltt {
namespace AST {

enum NodeType : uint8_t {
    TyLitNum,
    TyLitDec,
    TyIdent,
    TyType,
    TyExprBin,
    TyExprDecl,
};

struct Node;

struct NodeLitNum {
    int32_t val;
};

struct NodeLitDec {
    double val;
};

struct NodeIdent {
    const char* raw;
};

struct NodeTy {
    Node* ident;
    Node* next{};
};

struct NodeExprBin {
    Tok::TokID op;
    Node* lhs;
    Node* rhs;
};

struct NodeExprDecl {
    bool mut;
    Node* ident;
    Node* type;
    Node* expr;
};

struct Node {
    NodeType type;
    Tok::Token tok;

    union {
        NodeLitNum lit_num;
        NodeLitDec lit_dec;
        NodeIdent ident;
        NodeTy ty;
        NodeExprBin expr_bin;
        NodeExprDecl expr_decl;
    } data;

};

auto free_node(Node* _node) -> void;
auto inline dump_indent(std::ostream& _os, size_t _level) -> void
{
    _level *= 2;
    while (_level) { _os.write(" ", 1); --_level; }
}

auto dump(std::ostream& _os, Node* _node, const size_t _level = 0) -> void;

} // namespace AST
} // namespace Voltt