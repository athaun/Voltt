#include "parser.hpp"

namespace Voltt {
namespace Parser {

auto curr_t(CTX* _ctx) -> Tok::Token&
{
	return _ctx->tok_buf[_ctx->tok_pos];
}

auto next_t(CTX* _ctx) -> Tok::Token*
{
	if (_ctx->tok_pos+1 >= _ctx->tok_buf.size()) return nullptr;
	
	if (++_ctx->tok_pos >= _ctx->tok_buf.size()-1) _ctx->tok_lookahead = nullptr;
	else _ctx->tok_lookahead = &_ctx->tok_buf[_ctx->tok_pos+1]; 			

	return &_ctx->tok_buf[_ctx->tok_pos];
}

auto next_expecting(CTX* _ctx, const Tok::TokID _id) -> Tok::Token*
{
	Tok::Token* next_opt = next_t(_ctx);
	if (next_opt != nullptr && next_opt->id == _id) return next_opt;
	--_ctx->tok_pos;
	return nullptr;
}

auto peek_expecting(CTX* _ctx, const Tok::TokID _id) -> const bool
{
	return (_ctx->tok_lookahead != nullptr && _ctx->tok_lookahead->id == _id) ? true : false;
}

auto alloc_node() -> ASTNode::Node*
{
	return (ASTNode::Node*)std::malloc(sizeof(ASTNode::Node));
}

auto parse(CTX* _ctx) -> void
{
	for (;;) {
		if (curr_t(_ctx).id == Tok::TokenEndOfFile) return;
		ASTNode::Node* node = parse_toplevel_expr(_ctx);
		
		if (node == nullptr) return;
		next_t(_ctx); // consume EndOfStatement
		
		_ctx->body.emplace_back(std::move(node));
	}
}

auto parse_toplevel_expr(CTX* _ctx) -> ASTNode::Node*
{
	return parse_var_decl(_ctx); 
}

auto parse_expr(CTX* _ctx) -> ASTNode::Node*
{
	return parse_addative_expr(_ctx);
}

auto parse_primary_expr(CTX* _ctx) -> ASTNode::Node*
{
	switch (curr_t(_ctx).id) {
		case Tok::TokenParenOpen: 
			return parse_paren_expr(_ctx);

		case Tok::TokenIdent:
			return parse_ident(_ctx);

		default:
			return parse_literal(_ctx);
	}
}

auto parse_paren_expr(CTX* _ctx) -> ASTNode::Node*
{
	next_t(_ctx); // consume '('
	ASTNode::Node* expr = parse_expr(_ctx);
	next_t(_ctx); // consume ')' ERROR CASE
	return expr;
}

auto parse_multiplicative_expression(CTX* _ctx) -> ASTNode::Node*
{
	ASTNode::Node* left = parse_primary_expr(_ctx);

	for (;;) {
		
	const Tok::Token op = curr_t(_ctx);
	switch (op.id) {
		default:
			goto multiplicative_recurse_end; 
		case Tok::TokenBinOpMul:
		case Tok::TokenBinOpDiv:
			next_t(_ctx); // consume operator

			ASTNode::Node* right = parse_primary_expr(_ctx);

			ASTNode::Node* new_left = alloc_node();
			new_left->type = ASTNode::TypeExprBinary;
			new_left->data.expr_binary_data.op = op.id;
			new_left->data.expr_binary_data.left = left;
			new_left->data.expr_binary_data.right = right;

			left = new_left;
	}
	}	

	multiplicative_recurse_end:

	return left;
}

auto parse_addative_expr(CTX* _ctx) -> ASTNode::Node*
{
	ASTNode::Node* left = parse_multiplicative_expression(_ctx);
	for (;;) {
	
	const Tok::Token op = curr_t(_ctx);
	switch (op.id) {
		default: goto addative_recurse_end; 
		case Tok::TokenBinOpAdd:
		case Tok::TokenBinOpSub:
			next_t(_ctx); // consume operator

			ASTNode::Node* right = parse_multiplicative_expression(_ctx);

			ASTNode::Node* new_left = alloc_node();
			new_left->type = ASTNode::TypeExprBinary;
			new_left->data.expr_binary_data.op = op.id;
			new_left->data.expr_binary_data.left = left;
			new_left->data.expr_binary_data.right = right;

			left = new_left;
	}
	}

	addative_recurse_end:

	return left;
}

auto parse_var_decl(CTX* _ctx) -> ASTNode::Node*
{
	ASTNode::Node* variable = alloc_node();
	variable->type = ASTNode::TypeVariableDecl;
	variable->data.variable_decl_data.ident = parse_ident(_ctx);

	switch (curr_t(_ctx).id) {
		default: Logger::unhandled_case_err("Expected a ':'");
		
		case Tok::TokenColonSymbol: 
			next_t(_ctx); // consume ':'
			variable->data.variable_decl_data.type = parse_type(_ctx);

			switch (curr_t(_ctx).id) {
				default: Logger::unhandled_case_err("Expected a ':' or '='");
				
				case Tok::TokenEqSymbol:
					variable->data.variable_decl_data.is_const = false;
					break;

				case Tok::TokenColonSymbol:
					variable->data.variable_decl_data.is_const = true;
					break;
			}

			next_t(_ctx);
			break;

		case Tok::TokenColonInferConst:
			variable->data.variable_decl_data.is_const = true;
			variable->data.variable_decl_data.type = nullptr;
			next_t(_ctx);
			break;

		case Tok::TokenColonInferMut:
			variable->data.variable_decl_data.is_const = false;
			variable->data.variable_decl_data.type = nullptr;
			next_t(_ctx);
			break;
	}

	variable->data.variable_decl_data.expr = parse_expr(_ctx);

	return variable;
}

auto parse_type(CTX* _ctx) -> ASTNode::Node*
{
	switch (curr_t(_ctx).id) {
		default: Logger::unhandled_case_err("Expected a valid type Identifier");
		
		case VTYPES_CASE:
		case Tok::TokenIdent:
			ASTNode::Node* type = alloc_node();
			type->type = ASTNode::TypeTy;
			type->data.ty_data.ty = parse_ident(_ctx);

			return type;
	}

	Logger::unreachable_err();
	return nullptr;
}

auto parse_literal(CTX* _ctx) -> ASTNode::Node*
{
	switch (curr_t(_ctx).id) {
		default:
			Logger::unhandled_case_err("Expected a literal");
			return nullptr;
	
		case Tok::TokenLiteralNumeric: return parse_literal_numeric(_ctx);
		case Tok::TokenLiteralDecimal: return parse_literal_decimal(_ctx);
	
	}
	
	Logger::unreachable_err();
	return nullptr;
}

auto parse_literal_numeric(CTX* _ctx) -> ASTNode::Node*
{
	ASTNode::Node* literal_numeric = alloc_node();
	literal_numeric->type = ASTNode::TypeLiteralNumeric;
	
	const char* literal_numeric_raw = Tok::to_str(curr_t(_ctx), _ctx->contents);

	literal_numeric->data.literal_numeric_data.value = std::stoi(literal_numeric_raw);
	next_t(_ctx);

	std::free((void*)literal_numeric_raw);

	return literal_numeric;
}

auto parse_literal_decimal(CTX* _ctx) -> ASTNode::Node*
{
	ASTNode::Node* literal_decimal = alloc_node();
	literal_decimal->type = ASTNode::TypeLiteralDeciamal;

	const char* literal_decimal_raw = Tok::to_str(curr_t(_ctx), _ctx->contents);

	literal_decimal->data.literal_decimal_data.value = std::stold(literal_decimal_raw);
	next_t(_ctx);

	std::free((void*)literal_decimal_raw);
	
	return literal_decimal;
}

auto parse_ident(CTX* _ctx) -> ASTNode::Node*
{
	ASTNode::Node* ident = alloc_node();
	ident->type = ASTNode::TypeIdent;
	ident->data.ident_data.raw = Tok::to_str(curr_t(_ctx), _ctx->contents); 

	next_t(_ctx);

	return ident;
}

} // namespace Parser
} // namespace Voltt
