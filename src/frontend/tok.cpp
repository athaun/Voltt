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
	
		case TokenColonSymbol: return "TokenColonSymbol";
		case TokenEqSymbol: return "TokenEqSymbol";
		case TokenUndefinedIdent: return "TokenUndefinedIdent";
		case TokenTypI32: return "TokenTypI32";
		case TokenLiteralI32: return "TokenLiteralI32";
		case TokenEndOfFile: return "TokenEndOfFile";
		case TokenNewline: return "TokenNewline";
	}
	Logger::debug(Logger::DBCTX, Logger::DebugErrID::DebugErrID_t::UNREACHABLE_ERR, "This wasn't supposed to happen...");
}

auto operator<<(std::ostream& _os, const Token& _tok) -> std::ostream& 
{
	_os << "Token {\n";
	_os << "  m_id: " << to_str(_tok.m_id) << '\n';
	_os << "  m_start: " << _tok.m_start << '\n';
	_os << "  m_end: " << _tok.m_end << '\n';
	_os << "  m_line: " << _tok.m_line << '\n';
	_os << "  m_col: " << _tok.m_col << '\n';
	_os << '}';
	return _os;
}

} // namespace Tok
} // namespace Voltt
