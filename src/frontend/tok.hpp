#pragma once

#include "../logger.hpp"

namespace Voltt {
namespace Tok {

enum TokID {

    TokenColonSymbol,
    TokenEqSymbol,

    TokenUndefinedIdent,

    TokenTypI32,

    TokenLiteralI32,

    TokenEndOfFile,
    TokenNewline,
};

auto to_str(const TokID) -> const char*;

struct Token {
	TokID m_id;

	uint32_t m_start;
	uint32_t m_end;
	uint32_t m_line;
	uint32_t m_col;
};


auto operator<<(std::ostream& _os, const Token& _tok) -> std::ostream&;

} // namespace Tok
} // namespace Voltt
