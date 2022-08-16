#pragma once

#include "tok.hpp"
#include "../defer.hpp"

#include <cstdlib>

namespace Voltt {
namespace ASTNode {
	
enum NodeType : uint8_t {
	TypeExprBinary,
	TypeLiteralNumeric,
	TypeLiteralDeciamal,
	TypeIdent,
	TypeVariableDecl,
};

struct Node;

struct NodeLiteralNumeric {
	int64_t value;
};

struct NodeLiteralDecimal {
	long double value;
};

struct NodeExprBinary {
	Tok::TokID op;
	Node* left;
	Node* right;
};

struct NodeIdent {
	const char* raw;
};

struct NodeVariableDecl {
	bool is_const = true;

	Node* ident;
	Node* type;
	Node* expr;
};

struct Node {
	NodeType type;

	union {
		NodeLiteralNumeric literal_numeric_data;
		NodeLiteralDecimal literal_decimal_data;
		NodeExprBinary expr_binary_data;
		NodeIdent ident_data;
		NodeVariableDecl variable_decl_data;
	} data;
};

} // namespace ASTNode
} // namespace Voltt
