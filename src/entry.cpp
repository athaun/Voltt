#include "frontend/tokenizer.hpp"
#include "frontend/parser.hpp"
#include "logger.hpp"

#include <iostream>
#include <cassert>

auto main() -> int32_t
{
	using namespace Voltt;

	Tokenizer::TokenizerCTX tokctx("test.vlt");
	Tokenizer::tokenize(&tokctx);

	Parser::ParserCTX parctx(&tokctx);

	while (Parser::parse_primary_expr(&parctx));

	return 0;
}
