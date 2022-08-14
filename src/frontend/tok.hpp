#pragma once

#include "../logger.hpp"

#include <cstring>

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

    TokenTypS32,
   
	TokenIdent,
    TokenLiteralNumeric,

    TokenEndOfFile,
};

#define ALLOC_STR_CASE \
	TokenIdent: \
	case TokenLiteralNumeric \

struct Token {
	TokID id;

	size_t offset;
	size_t end; 
	size_t line;
	size_t col;

	explicit Token(const TokID _id, const size_t _offset, const size_t _line, const size_t _col)
	: id(_id), offset(_offset), line(_line), col(_col), end(0)
	{}
};

auto to_str(const Token&, const char*) -> const char*;
auto dump(std::ostream&, const Token&, const char*) -> void; 

} // namespace Tok
} // namespace Voltt
