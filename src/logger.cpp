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
		case TODO_ERR: return "TODO";
		case UNREACHABLE_ERR: return "Encountered an unreachable condition during compilation";
		case UNHANDLED_CASE_ERR: return "Encountered an unrecoverable conditional error";
		case TOKID_STR_ERR: return "Encountered an error converting Tok::TokID to string";
		case TOK_STATE_ERR: return "Encountered an error determining the tokenizer state";
		case ASTGEN_INVALID_TOK_ERR: return "Encountered an unhandled token during AST generation";
		case UNWRAP_ERR: return "Failed to unwrap a value, try being safe next time.";
	}

	abort();
}

} // namespace DebugErrID

// REFACTOR THIS. THIS IS AWFUL
auto operator<<(std::ostream& _os, const CompCtx_t& _ctx) -> std::ostream&
{

	const std::string line_num = std::to_string(_ctx.m_line);
	const size_t line_len = (_ctx.m_line_end-_ctx.m_line_start);

	_os << " --> " << _ctx.m_fname << ':' << _ctx.m_line << ':' << _ctx.m_start+1 << '\n';

	for (size_t idx = 0; idx <= line_num.length(); ++idx) _os.write(" ", 1);
	_os.write("|\n", 2);

	_os << line_num << " | ";
	
	_os.write(&_ctx.m_fd_contents[_ctx.m_line_start], line_len);
	_os.write("\n", 1);

	for (size_t idx = 0; idx <= line_num.length(); ++idx) _os.write(" ", 1);
	_os << "| ";
	
	size_t tok_offset = (_ctx.m_start-_ctx.m_line_start);
	for (; tok_offset-- ;) _os.write(" ", 1);

	switch (_ctx.m_end) {
		default:
			for (size_t idx = _ctx.m_start; idx <= _ctx.m_end; idx++) _os.write("^", 1);
			break;

		case 0: _os.write("^", 1);
	}

	_os.write("\n", 1);

	return _os;
}

namespace CompErrID {

auto to_str(const CompErrID_t _id) -> const char*
{
	switch (_id) {
		default: abort();
	
		case UNKNOWN: return "Unknown Error";
		case UNKNOWN_FILE_ERR: return "Invalid File to Compile";
		case INVALID_FILE_EXTENSION_ERR: return "Invalid File Extension Error";
		case READ_FILE_ERR: return "Input File Error";

		case INVALID_DECIMAL_FORMAT: return "Invalid Decimal Format";
		case INVALID_TYPE_IDENTIFIER: return "Invalid Type Identifer";
		case INVALID_TOPLEVEL_EXPR: return "Invalid Top Level Expression";

		case EXPECTED_LITERAL: return "Expected Literal";
		case EXPECTED_EQ: return "Expected Assignment";
		case EXPECTED_DECLARATION: return "Expected Declaration";
		case EXPECTED_NEWLINE: return "Expected Newline";
		case EXPECTED_EXPRESSION: return "Expected Expression";

		case UNMATCHED_OPEN_PAREN: 
		case UNMATCHED_CLOSE_PAREN: return "Unmatched parenthesis";
	}

	abort();
}

auto to_msg(const CompErrID_t _id) -> const char*
{
	switch (_id) {
		default: abort();
	
		case UNKNOWN: return "Encountered an unknown error.";
		case UNKNOWN_FILE_ERR: return "Encountered an error trying to open a file.";
		case INVALID_FILE_EXTENSION_ERR: return "All voltt files must have the .vlt extension";
		case READ_FILE_ERR: return "Encountered an error reading from a file";
	
		case INVALID_DECIMAL_FORMAT: return "Numeric Literals cannot contain more than one decimal";
		case INVALID_TYPE_IDENTIFIER: return "Identifer is an invalid type";
		case INVALID_TOPLEVEL_EXPR: return "Must declare an expression at top level";

		case EXPECTED_LITERAL: return "Expected a literal but got an identifier instead";
		case EXPECTED_EQ: return "Expected either an ':' or '=' for assignment";
		case EXPECTED_DECLARATION: return "Use ':', '::', or ':=' for variable assignment";
		case EXPECTED_NEWLINE: return "All expressions must end with a newline.";
		case EXPECTED_EXPRESSION: return "No expression to assign.";

		case UNMATCHED_CLOSE_PAREN: return "Unmatched closing parenthasises or however you spell them.";
		case UNMATCHED_OPEN_PAREN: return "Unmatched open parenthesis";
		
	}

	abort();
}

} // namespace CompErrID

auto cmperr(const CompCtx_t _ctx, const CompErrID::CompErrID_t _id) -> void
{
	std::cerr << "ERR[E" << std::to_string(_id) << "] " << CompErrID::to_str(_id) << '\n';
	std::cerr << _ctx;

	std::cerr << "MSG!\n"; 
	std::cerr << "    \"" << CompErrID::to_msg(_id) << "\"\n";
	std::cerr << std::endl;

	exit(1);
}

auto cmperr(const CompErrID::CompErrID_t _id) -> void
{
	std::cerr << "ERR[E" << std::to_string(_id) << "] " << CompErrID::to_str(_id) << '\n';
	
	std::cerr << "MSG!\n"; 
	std::cerr << "    \"" << CompErrID::to_msg(_id) << "\"\n";
	std::cerr << std::endl;

	exit(1);
}

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
