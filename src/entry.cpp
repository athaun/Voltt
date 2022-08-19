#include "frontend/parser.hpp"
#include "frontend/astgen.hpp"

#include <iostream>
#include <cstdint>

auto main() -> int32_t
{
	using namespace Voltt;

	Tokenizer::CTX tokctx("test.vlt");
	Tokenizer::tokenize(&tokctx);

	Parser::CTX parctx(&tokctx);
	Parser::parse(&parctx);
	
	for ( const ASTNode::Node* node : parctx.body ) ASTGen::ast_dump_node(std::cout, node);

	return 0;
}
