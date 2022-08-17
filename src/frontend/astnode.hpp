#pragma once

#include "tok.hpp"
#include "../defer.hpp"

#include <cstdlib>

namespace Voltt {
namespace ASTNode {
	
enum NodeType : uint8_t {
	TypeStatement,
	TypeVariableDecl,
	TypeExprBinary,
	TypeTy,
	TypeLiteralNumeric,
	TypeLiteralDeciamal,
	TypeIdent,
};

struct Node;

struct NodeStatement {
	Node* expr;
};

struct NodeVariableDecl {
	bool is_const;

	Node* ident;
	Node* type;
	Node* expr;
};

struct NodeExprBinary {
	Tok::TokID op;
	Node* left;
	Node* right;
};

struct NodeTy {
	Node* ty;
};

struct NodeLiteralNumeric {
	int64_t value;
};

struct NodeLiteralDecimal {
	long double value;
};


struct NodeIdent {
	const char* raw;
};

struct Node {
	NodeType type;

	union {
		NodeLiteralNumeric literal_numeric_data;
		NodeLiteralDecimal literal_decimal_data;
		NodeExprBinary expr_binary_data;
		NodeTy ty_data;
		NodeIdent ident_data;
		NodeVariableDecl variable_decl_data;
		NodeStatement statement_data;
	} data;
};

} // namespace ASTNode
} // namespace Voltt
