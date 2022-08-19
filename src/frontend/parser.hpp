#pragma once

#include "tokenizer.hpp"
#include "astnode.hpp"
#include "astgen.hpp"

namespace Voltt {
namespace Parser {

struct CTX;

auto curr_t(CTX*) -> Tok::Token&;
auto next_t(CTX*) -> Tok::Token*;
auto next_expecting(CTX, const Tok::TokID) -> Tok::Token*;
auto peek_expecting(CTX, const Tok::TokID) -> const bool;

auto alloc_node() -> ASTNode::Node*;

auto parse(CTX*) -> void;

auto parse_toplevel_expr(CTX*) -> ASTNode::Node*;

/*
 * Expression
*/
auto parse_expr(CTX*) -> ASTNode::Node*;

/*
 * PrimaryExpression
 *	: Literal
 *	| Parenthasized Expression
 *	;
*/
auto parse_primary_expr(CTX*) -> ASTNode::Node*;

/*
 * Parenthasized Expression
 *	: '(' Expression ')'
 *	;
*/
auto parse_paren_expr(CTX*) -> ASTNode::Node*;

/*
 * Literal
 *	: Integer Literal
 *	| Decimal Literal
 *	;
*/
auto parse_literal(CTX*) -> ASTNode::Node*;

/*
 * Type
 *	: Ident -> ...
 *	;
*/
auto parse_type(CTX*) -> ASTNode::Node*;

/*
 * Literal Numeric
 *	: Integer Literal
 *	;
*/
auto parse_literal_numeric(CTX*) -> ASTNode::Node*;

/*
 * Literal Numeric
 *	: Decimal Literal
 *	;
*/
auto parse_literal_decimal(CTX*) -> ASTNode::Node*;

/*
 * Ident
 *	;
*/
auto parse_ident(CTX*) -> ASTNode::Node*;

/*
 * Multiplicative Expression
 *	: Multiplicative Expression Operator Multiplicative Expression -> ...
*/

auto parse_multiplicative_expression(CTX*) -> ASTNode::Node*;

/*
 * Addative Expression
 *  : Multiplicative Expression
 *	| Addative Expression Operator Multiplicative Expression <|> AddativeExpression -> ...
 *	;
*/
auto parse_addative_expr(CTX*) -> ASTNode::Node*;

/*
 * Variable Declaration
 *	: Variable -> Ident -> Type -> Expression
 *	;
*/ 
auto parse_var_decl(CTX*) -> ASTNode::Node*;

struct CTX {
	size_t tok_pos;
	Tok::Token* tok_lookahead;

	std::vector<Tok::Token> tok_buf;
	std::vector<ASTNode::Node*> body;

	const char* fname;
	const char* contents;

	CTX(Tokenizer::CTX* _t)
	: tok_pos(0),
		tok_buf(std::move(_t->tok_buf)),
		fname(_t->fname),
		contents(std::move(_t->contents))
	{
		tok_lookahead = &tok_buf[1];
	}

	~CTX()
	{
		tok_lookahead = nullptr;
		
		//std::free((void*)fname);
		fname = nullptr;

		std::free((void*)contents);
		contents = nullptr;

		for ( ASTNode::Node* node : body ) ASTGen::ast_free_node(node);
		body[0] = nullptr;
	}
};


} // namespace Parser
} // namespace Voltt
