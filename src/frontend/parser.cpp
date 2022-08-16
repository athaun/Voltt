#include "parser.hpp"
#include "tok.hpp"

namespace Voltt {
namespace Parser {

auto next_t(ParserCTX* _p) -> const bool 
{
	if (_p->tok_pos >= _p->tok_buf.size()-1) return false;
	_p->tok_curr = _p->tok_buf[++_p->tok_pos];
	return true;
}

auto next_expecting(ParserCTX* _p, const Tok::TokID _id) -> const bool
{
	if (_p->tok_pos >= _p->tok_buf.size()-1) return false;
	
	const Tok::Token* tok_potential = &_p->tok_buf[++_p->tok_pos];
	if (tok_potential->id != _id) return false;

	_p->tok_curr = *tok_potential;
	return true;
}

auto peek_t(const ParserCTX* _p, const size_t _offset) -> const Optional<Tok::Token>
{
	const size_t offset = (_p->tok_pos+_offset);
	if (offset >= _p->tok_buf.size()-1) return Optional<Tok::Token>{};

	return Optional<Tok::Token>(_p->tok_buf[offset]);
}

auto peek_expecting(const ParserCTX* _p, const size_t _offset, const Tok::TokID _id) -> const Optional<Tok::Token>
{
	const size_t offset = (_p->tok_pos+_offset);
	if (offset >= _p->tok_buf.size()-1) return Optional<Tok::Token>{};
	
	const Tok::Token* tok_potential = &_p->tok_buf[offset];
	if (tok_potential->id != _id) return Optional<Tok::Token>{};

	return Optional<Tok::Token>(*tok_potential);
}

auto parse_primary_expr(ParserCTX* _p) -> const bool
{
	Tok::dump(std::cout, _p->tok_curr, _p->contents);
	switch (_p->tok_curr.id) {
		default: Logger::unhandled_case_err("Invalid expression at top level.");
		
		case Tok::TokenIdent:
			_p->body.push_back(std::move(parse_var_decl(_p)));
			return true;
		
		case Tok::TokenEndStatement:
			next_t(_p); // consume end of statement
			return true;

		case Tok::TokenEndOfFile:
			return false;
	}

	Logger::unreachable_err();
	return false;
}

auto parse_expr(ParserCTX* _p) -> ASTNode::Node*
{
	ASTNode::Node* res;
	switch (_p->tok_curr.id) {
		default:
			res = parse_addative_expr(_p);
			break;

		case Tok::TokenParenOpen:
			res = parse_paren_expr(_p);
			break;
	}

	switch(_p->tok_curr.id) {
		default:
			Logger::unhandled_case_err("Invalid end of expression");
			return nullptr;

		case Tok::TokenEndStatement:
			next_t(_p); // consume end statement
		
		case Tok::TokenParenClose:
		case Tok::TokenEndOfFile:
			return res;
	
	}

	Logger::unreachable_err();
	return nullptr;
}

auto parse_paren_expr(ParserCTX* _p) -> ASTNode::Node*
{
	next_t(_p); // consume '('
	ASTNode::Node* expr = parse_expr(_p);
	next_t(_p); // consume ')' ERROR CASE
	
	return expr;
}

auto parse_addative_expr(ParserCTX* _p) -> ASTNode::Node*
{
	ASTNode::Node* left = parse_literal_numeric(_p); 
	
	for (;;) {
	switch (_p->tok_curr.id) {
		default: goto addative_recurse_end;
			
		case BINOP_CASE:

			const Tok::TokID op = _p->tok_curr.id;
				
			next_t(_p); // consume literal ERROR CASE
			ASTNode::Node* right = parse_literal_numeric(_p);

			ASTNode::Node* new_left = (ASTNode::Node*)std::malloc(sizeof(ASTNode::Node));
				
			new_left->type = ASTNode::TypeExprBinary;
			new_left->data.expr_binary_data.op = op;
			new_left->data.expr_binary_data.left = left;
			new_left->data.expr_binary_data.right = right;
				
			left = new_left;
	}
	}

	addative_recurse_end:
	return left;
}

auto parse_literal(ParserCTX* _p) -> ASTNode::Node*
{
	switch(_p->tok_curr.id) {
		default: Logger::unhandled_case_err("Expected a literal");
		
		case Tok::TokenLiteralNumeric:
		case Tok::TokenLiteralDecimal:
			return parse_literal_numeric(_p);
	}

	Logger::unreachable_err();
	return nullptr;
}

auto parse_literal_numeric(ParserCTX* _p) -> ASTNode::Node* 
{
	ASTNode::Node* num = (ASTNode::Node*)std::malloc(sizeof(ASTNode::Node));
	const char* literal_word = Tok::to_str(_p->tok_curr, _p->contents);
	
	switch(_p->tok_curr.id) {
		default:
			Logger::unreachable_err();
			return nullptr;

		case Tok::TokenLiteralNumeric:
			num->type = ASTNode::TypeLiteralNumeric;
			num->data.literal_numeric_data = ASTNode::NodeLiteralNumeric{
				.value = std::stoi(literal_word),
			};
			
			break;

		case Tok::TokenLiteralDecimal:
			num->type = ASTNode::TypeLiteralDeciamal;
			num->data.literal_decimal_data = ASTNode::NodeLiteralDecimal{
				.value = std::stod(literal_word),
			};

			break;
	}

	std::free((void*)literal_word);

	next_t(_p); // consume numeric literal

	return num;
}

auto parse_ident_decl(ParserCTX* _p) -> ASTNode::Node*
{
	ASTNode::Node* ident = (ASTNode::Node*)std::malloc(sizeof(ASTNode::Node));
	ident->type = ASTNode::TypeIdent;
	ident->data.ident_data.raw = Tok::to_str(_p->tok_curr, _p->contents);

	next_t(_p); // consume ident
	
	return ident;
}

auto parse_type(ParserCTX* _p) -> ASTNode::Node*
{
	switch (_p->tok_curr.id) {
		default:
			Logger::unhandled_case_err("Invalid type");
			return nullptr;

		case VTYPES_CASE:
			return parse_ident_decl(_p);
	}

	Logger::unreachable_err();
	return nullptr;
}

auto parse_var_decl(ParserCTX* _p) -> ASTNode::Node*
{
	ASTNode::Node* variable = (ASTNode::Node*)std::malloc(sizeof(ASTNode::Node));
	variable->type = ASTNode::TypeVariableDecl;

	Optional<Tok::Token> peek_opt = peek_expecting(_p, 1, Tok::TokenColonSymbol);
	if (peek_opt.none) Logger::unhandled_case_err("Expected ':'");	
	
	variable->data.variable_decl_data.ident = parse_ident_decl(_p);
	next_t(_p); // consume ':'
	variable->data.variable_decl_data.type = parse_type(_p);

	switch (_p->tok_curr.id) {
		default:
			Logger::unhandled_case_err("Expected ':' or '='");
			return nullptr;

		case Tok::TokenColonSymbol:
			break;
		
		case Tok::TokenEqSymbol:
			variable->data.variable_decl_data.is_const = false;
			break;
	}

	next_t(_p); // consume expr
	variable->data.variable_decl_data.expr = parse_expr(_p);

	return variable;

	Logger::unreachable_err();
}

} // namespace Voltt
} // namespace Parser
