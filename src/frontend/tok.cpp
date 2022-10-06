#include "tok.hpp"

namespace Voltt {
namespace Tok {

auto dump(std::ostream& _os, const Token& _tok, const char* _source) -> void
{
	const char* word = to_str(_tok, _source);
	_os << "Token {\n";
	_os << "  word: " << word << '\n';
	_os << "  offset: " << _tok.offset << '\n';
	_os << "  end: " << _tok.end << '\n';
	_os << "  line: " << _tok.line << '\n';
	_os << "  col: " << _tok.col << '\n';
	_os << "}\n";

	// safely deallocates memory
	switch (_tok.id) {
		case ALLOC_STR_CASE: std::free(const_cast<char*>(word));
		default: return;
	}
}

auto dump_errctx(const Tok::Token& _tok, const char* _contents, const char* _fname) -> const Logger::CompCTX
{
	size_t line_start = _tok.offset;
	size_t line_end = _tok.end;

	for (;;) if (line_start <= 0 || _contents[--line_start] == '\n') break;

	for (;;) if (line_end >= strlen(_contents) || _contents[++line_end] == '\n') break;

	return {
		.start = _tok.offset,
		.end = _tok.end,
		.line_start = line_start,
		.line_end = line_end,
		.line = _tok.line-1,
		.fname = _fname,
		.fd_contents = _contents,
	};
}

auto to_str(const Tok::Token& _tok, const char* _source) -> char const*
{
	switch(_tok.id) {
		default: Logger::tokid_str_err(DBCTX);

		case TokenColonSymbol: return ":";
		case TokenColonInferMut: return ":=";
		case TokenColonInferConst: return "::";
		case TokenEqSymbol: return "=";

		case TokenCmpGreater: return ">";
		case TokenCmpGreaterEq: return ">=";
		case TokenCmpLess: return "<";
		case TokenCmpLessEq: return "<=";

		case TokenArrowRight: return "->";
		case TokenArrowLeft: return "<-";

		case TokenComma: return ",";

		case TokenParenOpen: return "(";
		case TokenParenClose: return ")";

		case TokenBracketOpen: return "{";
		case TokenBracketClose: return "}";	

		case TokenBinOpAdd: return "+";
		case TokenBinOpSub: return "-";
		case TokenBinOpMul: return "*";
		case TokenBinOpDiv: return "/";

		case TokenTypS32: return "s32";

		case TokenFn: return "fn";
		case TokenRet: return "ret";
		
		case TokenEndStatement: return "TokenEndStatement";
		case TokenEndOfFile: return "TokenEndOfFile";

		// In order to prevent memory leaks the result of this function must follow this pattern
		// switch (Tok::Token.id) {
		//	  case Tok::ALLOC_STR_CASE: std::free(const_cast<char*>(word));
		//	  default break;
		// }
		// This solution is hacky, but it can greatly reduce the amount of allocations and copies.
		// Look into defer.hpp for nicer looking ways to clean up	

		case ALLOC_STR_CASE:
			size_t len = (_tok.end-_tok.offset)+1;
			char* result = static_cast<char*>(std::malloc(len));
			std::memcpy(result, &_source[_tok.offset], len);
			result[len] = 0;
			return result;
	}
	Logger::unreachable_err(DBCTX);
}

} // namespace Tok
} // namespace Voltt
