#include "logger.hpp"

namespace Logger {

auto operator<<(std::ostream& _os, const DebugCtx_t& _ctx) -> std::ostream&
{
	_os << "  --> " << _ctx.m_file << ':' << _ctx.m_func << "():" << _ctx.m_line;
	return _os; 
}

namespace DebugErrID {

auto to_str(const DebugErrID_t _id) -> const char*
{
	switch (_id) {
		default: abort();

		case UNKNOWN: return "Encountered an unknown error during compilation";	
		case TODO: return "TODO";
		case UNREACHABLE_ERR: return "Encountered an unreachable condition during compilation";
		case TOKID_STR_ERR: return "Encountered an error converting Tok::TokID to string";
	}

	abort();
}

} // namespace DebugErrID

auto operator<<(std::ostream& _os, const CompCtx_t& _ctx) -> std::ostream&
{
	_os << "[std::ostream& << CompCtx_t NOT IMPLEMENTED]" << std::endl;
	return _os;
}

namespace CompErrID {

auto to_str(const CompErrID_t _id) -> const char*
{
	switch (_id) {
		default: abort();
		
		case UNKNOWN: return "Encountered an unknown error during compilation";
	}

	abort();
}

} // namespace CompErrID
} // namespace Logger
