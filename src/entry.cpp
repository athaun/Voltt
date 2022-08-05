#include "logger.hpp"
#include "frontend/tok.hpp"

#include <iostream>

auto main() -> int32_t
{
	using namespace Voltt;

	Tok::Token tok{ .m_id = Tok::TokenTypI32, };
	std::cout << tok << std::endl;
	return 0;
}
