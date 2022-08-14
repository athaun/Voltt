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
		case TOK_STATE_ERR: return "Encountered an error determining the tokenizer state";
		case ASTGEN_INVALID_TOK_ERR: return "Encountered an unhandled token during AST generation";
		case UNWRAP_ERR: return "Failed to unwrap a value, try being safe next time.";
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
		case UNKNOWN_FILE_ERR: return "Invalid file to compile.";
		case INVALID_FILE_EXTENSION_ERR: return "Voltt files must end in a valid extension: .vlt";
		case READ_FILE_ERR: return "Encountered an error reading from an input file.";
	}

	abort();
}

} // namespace CompErrID

auto read_file_err(const char* _fname) -> void
{
	_os << "ERR![E" << std::to_string(CompErrID::CompErrID_t::READ_FILE_ERR) << "]: " << CompErrID::to_str(CompErrID::CompErrID_t::READ_FILE_ERR) << '\n';
	_os << "  --> " << _fname << std::endl; 

	exit(1);
}

auto invalid_extension_err(const char* _fname) -> void
{
	_os << "ERR![E" << std::to_string(CompErrID::CompErrID_t::INVALID_FILE_EXTENSION_ERR) << "]: " << CompErrID::to_str(CompErrID::CompErrID_t::INVALID_FILE_EXTENSION_ERR) << '\n';
	_os << "  --> " << _fname << std::endl;

	exit(1);
}

auto invalid_file_err(const char* _fname) -> void
{
	_os << "ERR![E" << std::to_string(CompErrID::CompErrID_t::READ_FILE_ERR) << "]: " << CompErrID::to_str(CompErrID::CompErrID_t::READ_FILE_ERR) << '\n';
	_os << "  --> " << _fname << std::endl; 

	exit(1);
}

} // namespace Logger
