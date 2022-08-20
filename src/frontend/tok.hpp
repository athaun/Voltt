#pragma once

#include "../logger.hpp"

#include <cstring>
#include <cstdint>

namespace Voltt {
namespace Tok {

enum TokID : uint8_t {

    TokenColonSymbol,
	TokenColonInferMut,
	TokenColonInferConst,
    TokenEqSymbol,

	TokenCmpGreater,
	TokenCmpGreaterEq,
	TokenCmpLess,
	TokenCmpLessEq,

	TokenArrowRight,
	TokenArrowLeft,

	TokenComma,

	TokenParenOpen,
	TokenParenClose,

	TokenBracketOpen,
	TokenBracketClose,

	TokenBinOpAdd,
	TokenBinOpSub,
	TokenBinOpMul,
	TokenBinOpDiv,

    TokenTypS32,

	TokenFn,
   
	TokenIdent,
    TokenLiteralNumeric,
	TokenLiteralDecimal,

	TokenEndStatement,
    TokenEndOfFile,
};

#define ALLOC_STR_CASE \
	Tok::TokenIdent: \
	case Tok::TokenLiteralNumeric: \
	case Tok::TokenLiteralDecimal

#define BINOP_CASE \
	Tok::TokenBinOpAdd: \
	case Tok::TokenBinOpSub: \
	case Tok::TokenBinOpMul: \
	case Tok::TokenBinOpDiv

#define VTYPES_CASE \
	Tok::TokenTypS32

struct Token {
	TokID id;

	size_t offset;
	size_t end; 
	size_t line;
	size_t col;

	Token() = default;

	explicit Token(const TokID _id, const size_t _offset, const size_t _line, const size_t _col)
	: id(_id), offset(_offset), line(_line), col(_col), end(0)
	{}
};

auto dump(std::ostream&, const Token&, const char*) -> void; 
auto dump_errctx(const Token&, const char*, const char*) -> Logger::CompCtx_t;
auto to_str(const Token&, const char*) -> const char*;

} // namespace Tok
} // namespace Voltt
