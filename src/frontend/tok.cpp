#include "tok.hpp"

namespace Voltt {
namespace Tok {

auto to_str(const TokID _id) -> const char*
{
	switch(_id) {
		default: 
			Logger::debug(
				Logger::DBCTX,
				Logger::DebugErrID::DebugErrID_t::TOKID_STR_ERR,
				std::string{"Unable to convert: "}+std::to_string(_id)+" to str."
			);
	
		case TokenColonSymbol: return ":";
		case TokenColonInferMut: return ":=";
		case TokenColonInferConst: return "::";
		case TokenEqSymbol: return "=";

		case TokenBinOpAdd: return "+";
		case TokenBinOpSub: return "-";
		case TokenBinOpMul: return "*";
		case TokenBinOpDiv: return "/";

		case TokenIdent: return "TokenIdent";
		
		case TokenTypS32: return "s32";
		
		case TokenLiteralNumeric: return "TokenLiteralNumeric";
		
		case TokenEndOfFile: return "TokenEndOfFile";
	}
	Logger::debug(Logger::DBCTX, Logger::DebugErrID::DebugErrID_t::UNREACHABLE_ERR, "This wasn't supposed to happen...");
}

auto operator<<(std::ostream& _os, const Token& _tok) -> std::ostream& 
{
	_os << "Token {\n";
	_os << "  id: " << to_str(_tok.id) << '\n';
	_os << "  offset: " << _tok.offset << '\n';
	_os << "  line: " << _tok.line << '\n';
	_os << "  col: " << _tok.col << '\n';
	_os << '}';
	return _os;
}

} // namespace Tok
} // namespace Voltt
