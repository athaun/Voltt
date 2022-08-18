#include "frontend/tokenizer.hpp"
#include "frontend/astgen.hpp"
#include "frontend/parser.hpp"
#include "logger.hpp"

#include <iostream>
#include <cassert>

auto main() -> int32_t
{
	using namespace Voltt;
	
	Tokenizer::TokenizerCTX tokctx("test.vlt");
	Tokenizer::tokenize(&tokctx);

	Parser::CTX parctx(&tokctx);

	Parser::parse(&parctx);

	return 0;
}
