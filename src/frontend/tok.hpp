#pragma once

#include "../logger.hpp"

namespace Voltt {
namespace Tok {

enum TokID : uint8_t {

    TokenColonSymbol,
	TokenColonInferMut,
	TokenColonInferConst,
    TokenEqSymbol,

	TokenBinOpAdd,
	TokenBinOpSub,
	TokenBinOpMul,
	TokenBinOpDiv,

    TokenIdent,

    TokenTypS32,

    TokenLiteralNumeric,

    TokenEndOfFile,
};

auto to_str(const TokID) -> const char*;

struct Token {
	TokID id;

	size_t offset;
	size_t line;
	size_t col;

	Token(const TokID _id, const size_t _offset, const size_t _line, const size_t _col)
	: id(_id), offset(_offset), line(_line), col(_col)
	{}
};


auto operator<<(std::ostream& _os, const Token& _tok) -> std::ostream&;

} // namespace Tok
} // namespace Voltt
