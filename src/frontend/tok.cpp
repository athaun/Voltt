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
		case ALLOC_STR_CASE: std::free((void*)word);
		default: return;
	}
}

auto to_str(const Tok::Token& _tok, const char* _source) -> const char*
{
	switch(_tok.id) {
		default: 
			Logger::debug(
				Logger::DBCTX,
				Logger::DebugErrID::DebugErrID_t::TOKID_STR_ERR,
				std::string{"Unable to convert: "}+std::to_string(_tok.id)+" to str."
			);
	
		case TokenColonSymbol: return ":";
		case TokenColonInferMut: return ":=";
		case TokenColonInferConst: return "::";
		case TokenEqSymbol: return "=";

		case TokenBinOpAdd: return "+";
		case TokenBinOpSub: return "-";
		case TokenBinOpMul: return "*";
		case TokenBinOpDiv: return "/";

		case TokenTypS32: return "s32";
		
		case TokenEndOfFile: return "TokenEndOfFile";

		// In order to prevent memory leaks the result of this function must follow this pattern
		// switch (Tok::Token.id) {
		//	  case Tok::ALLOC_STR_CASE: std::free((void*)word);
		//	  default break;
		// }
		// This solution is hacky, but it can greatly reduce the amount of allocations and copies.
		// Look into defer.hpp for nicer looking ways to clean up	

		case ALLOC_STR_CASE:
			size_t len = (_tok.end-_tok.offset)+1;
			char* result = (char*)std::malloc(len);
			std::memcpy(result, &_source[_tok.offset], len);
			result[len] = 0;
			return result;
		
	}

	Logger::debug(
		Logger::DBCTX,
		Logger::DebugErrID::UNREACHABLE_ERR,
		"This wasn't supposed to happen..."
	);
}

} // namespace Tok
} // namespace Voltt
