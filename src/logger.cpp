#include "logger.hpp"

namespace Logger {

static std::ostream& _os = std::cerr;

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

auto debug(const DebugCtx_t& _ctx, const DebugErrID::DebugErrID_t _id, const char* _msg) -> void
{
		_os << "\n!!! This is a compiler debug error used for compiler development !!!\n";
		_os << "!!! If you are not developing the Voltt compiler this is most likely a bug !!!\n\n";
		_os << "DBG![D" << std::to_string(_id) << "]: " << DebugErrID::to_str(_id) << '\n';
		_os << _ctx << '\n';
		_os << " |\n";
		_os << " | " << _msg << std::endl;
		_os << " |\n";

	abort();
}

auto cmperr(const CompCtx_t& _ctx, const CompErrID::CompErrID_t _id, const char* _msg) -> void
{
	_os << "ERR![E" << std::to_string(_id) << "]: " << CompErrID::to_str(_id) << '\n';
	_os << _ctx << '\n';
	_os << " |\n";
	_os << " | " << _msg << std::endl;
	_os << " |\n";

	exit(1);
}

} // namespace Logger
