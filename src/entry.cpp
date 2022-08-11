#include "frontend/tokenizer.hpp"
#include "frontend/tok.hpp"
#include "optional.hpp"
#include "logger.hpp"

#include <iostream>

auto main() -> int32_t
{
	using namespace Voltt;

	Tokenizer::TokenizerCTX tokenizer("test.vlt");
	while (tokenizer.state != Tokenizer::STATE_EOF) Tokenizer::next_t(&tokenizer);

	for ( Tok::Token& tok : tokenizer.tok_buf ) std::cout << tok << '\n';

	return 0;
}
