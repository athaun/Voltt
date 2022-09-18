#include "frontend/ast.hpp"
#include "frontend/parser.hpp"

#include <cstdint>

auto main() -> int32_t
{
	Voltt::Tokenizer::CTX tokctx("test.vlt");
	Voltt::Tokenizer::tokenize(&tokctx);
	Voltt::Parser::CTX parctx(&tokctx);

	Voltt::Parser::parse(&parctx);
	Voltt::AST::dump(std::cout, parctx.body.first(), 0);

	return 0;
}
