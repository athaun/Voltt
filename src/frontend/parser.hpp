#pragma once

#include "../optional.hpp"
#include "tokenizer.hpp"
#include "astnode.hpp"
#include "../todo.hpp"

#include <map>

namespace Voltt {
namespace Parser {

struct ParserCTX {
	size_t tok_pos;
	Tok::Token tok_curr;
	std::vector<Tok::Token> tok_buf;
	std::vector<ASTNode::Node*> body;
	const char* contents;

	ParserCTX(const Tokenizer::TokenizerCTX* _t)
	: tok_pos(0), tok_curr(_t->tok_buf[tok_pos]), tok_buf(std::move(_t->tok_buf)), contents(std::move(_t->contents))
	{}

	~ParserCTX()
	{
		std::free((void*)contents);
	}
};

auto next_t(ParserCTX*) -> const bool;
auto next_expecting(ParserCTX*, const Tok::TokID) -> const bool;
auto peek_t(const ParserCTX*, const size_t) -> const Optional<Tok::Token>;
auto peek_expecting(const ParserCTX*, const size_t, const Tok::TokID) -> const Optional<Tok::Token>;

/*
 * Expression
 *	: Literal
 *	: VariableDecl
 *	| ParenExpr
*/
auto parse_primary_expr(ParserCTX*) -> const bool;

/*
 * Expression
 *	: AddativeExpression
*/
auto parse_expr(ParserCTX*) -> ASTNode::Node*;

/*
 * ParenExpr
 *	: ( Expression )
 */
auto parse_paren_expr(ParserCTX*) -> ASTNode::Node*;

/*
 * Addative Expression
 *	: Literal
 *	| AddativeExpression Operator Literal -> Literal Operator Literal Operator Literal
 *	;
*/
auto parse_addative_expr(ParserCTX*) -> ASTNode::Node*;

/*
 * Expression
 *	: Literal
 *	;
*/
auto parse_literal(ParserCTX*) -> ASTNode::Node*;

/*
 * Expression
 *	: NumericLiteral
 *	;
*/
auto parse_literal_numeric(ParserCTX*) -> ASTNode::Node*;

/*
 * Ident
*/
auto parse_ident_decl(ParserCTX*) -> ASTNode::Node*;

/*
 * Type
*/
auto parse_type(ParserCTX*) -> ASTNode::Node*;

/*
 * VariableDeclaration
 *	: Ident -> Type -> Expr
*/
auto parse_var_decl(ParserCTX*) -> ASTNode::Node*;

} // namespace Parser
} // namespace Voltt
