#include "logger.hpp"
#include <cstdlib>
#include <ostream>

namespace Logger {

auto operator<<(std::ostream& _os, const DBGErrno _err) -> std::ostream&
{
     _os << "DBG![" << std::to_string(_err) << "]: " << errno_str(_err);
    return _os;
}

auto errno_str(const DBGErrno _err) -> const char*
{
    switch (_err) {
        default: break;

        case DBG_UNKNOWN_ERR: return "Unknown Error";
        case UNREACHABLE_ERR: return "Unreachable Error";
        case UNHANDLED_CASE_ERR: return "Unhandled Case Error";
        case TOKID_STR_ERR: return "Unknown Token to Stringify Error";
        case TODO_ERR: return "TODO Error";
    }

    dbg_msg();
    std::cerr << "Invalid error code passsed to convert to error message!\n";
    
    std::exit(1);
}

auto errno_str(const CMPErrno _err) -> const char*
{
    switch (_err) {
        default: break;

        case CMP_UNKNOWN_ERR: return "Unknown Error";
        case INVALID_FILE_ERR: return "Invalid File Error";
        case INVALID_FILE_EXT_ERR: return "Invalid File Extension Error";
        case READ_FILE_ERR: return "Read File Error";
        case INVALID_DECIMAL_FORMAT: return "Invalid Decimal Format Error";
        case INVALID_TOPLEVEL_EXPR: return "Invalid Toplevel Expression Error";
        case UNMATCHED_CLOSE_PAREN: return "Unmatched Closing Parenthesis Error";
        case UNMATCHED_OPEN_PAREN: return "Unmatched Open Parenthesis Error";
        case EXPECTED_EXPRESSION: return "Expected Expression Error";
        case EXPECTED_DECLARATION: return "Expected Declaration Error";
        case EXPECTED_EQ: return "Expected EQ Error";
        case INVALID_TYPE_IDENTIFIER: return "Invalid Type Identifier Error";
        case EXPECTED_LITERAL: return "Expected Literal Error";
    }

    dbg_msg();
    std::cerr << "Invalid error code passsed to convert to error message!\n";
    
    std::exit(1);
}

auto operator<<(std::ostream& _os, const CMPErrno _err) -> std::ostream&
{
    _os << "ERR![" << std::to_string(_err) << "]: " << errno_str(_err);
    return _os;
}

auto operator<<(std::ostream& _os, const DebugCTX& _ctx) -> std::ostream&
{
    _os << " --> " << _ctx.fname << " : " << _ctx.func << "() : " << _ctx.line << '\n'; 
    return _os;
}

auto operator<<(std::ostream& _os, const CompCTX& _ctx) -> std::ostream&
{
    _os << " --> " << _ctx.fname << " : " << _ctx.line << ':' << _ctx.start << '\n';
    return _os;
}

auto output_prog_line(const CompCTX& _ctx, std::ostream& _os) -> void
{
	const std::string line_num = std::to_string(_ctx.line);
	const size_t line_len = (_ctx.line_end-_ctx.line_start);

	for (size_t idx = 0; idx <= line_num.length(); ++idx) _os.write(" ", 1);
	_os.write("|\n", 2);

	_os << line_num << " | ";
	
	_os.write(&_ctx.fd_contents[_ctx.line_start], line_len);
	_os.write("\n", 1);

	for (size_t idx = 0; idx <= line_num.length(); ++idx) _os.write(" ", 1);
	_os << "| ";
	
	size_t tok_offset = (_ctx.start-_ctx.line_start);
	for (; tok_offset-- ;) _os.write(" ", 1);

	switch (_ctx.end) {
		default:
			for (size_t idx = _ctx.start; idx <= _ctx.end; idx++) _os.write("^", 1);
			break;

		case 0: _os.write("^", 1);
	}

	_os.write("\n", 1);
}

/*
 * Debug ERRS
*/
auto unknown_err(const DebugCTX& _ctx, std::ostream& _os) -> void
{
    gen_err("An unknown error occurred", _ctx, DBG_UNKNOWN_ERR, _os);
}

auto unreachable_err(const DebugCTX& _ctx, std::ostream& _os) -> void
{
    gen_err("An unreachable condition has been reached", _ctx, UNREACHABLE_ERR, _os);
}

auto unhandled_case_err(const char* _msg, const DebugCTX& _ctx, std::ostream& _os) -> void
{
    gen_err(_msg, _ctx, UNHANDLED_CASE_ERR, _os);
}

auto tokid_str_err(const DebugCTX& _ctx, std::ostream& _os) -> void
{
   gen_err("Failed to convert TokID to string", _ctx, TOKID_STR_ERR, _os);
}

/*
 * Comptime ERRS
*/
auto unknown_err(const CompCTX& _ctx, std::ostream& _os) -> void
{
    dbg_msg(_os);
    gen_err("An unknown error occurred", _ctx, CMP_UNKNOWN_ERR, _os);
}

auto invalid_file_err(const CompCTX& _ctx, std::ostream& _os) -> void
{
    _os << INVALID_FILE_ERR << '\n';
    _os << _ctx << "|\n";
    _os << "| An error occured when attempting to open: \"" << _ctx.fname << "\"\n";
    _os << "|\n";

    std::exit(1);
}

auto invalid_file_extension_err(const CompCTX& _ctx, std::ostream& _os) -> void
{
    _os << INVALID_FILE_EXT_ERR << '\n';
    _os << _ctx << "|\n";
    _os << "| File: \"" << _ctx.fname << "\" did not contain a valid '.vlt' extension.\n";
    _os << "|\n";

    std::exit(1);
}

auto read_file_err(const CompCTX& _ctx, std::ostream& _os) -> void
{
    _os << READ_FILE_ERR << '\n';
    _os << _ctx << "|\n";
    _os << "| Unable to read file: \"" << _ctx.fname << "\"\n";
    _os << "|\n";

    std::exit(1);
}

auto invalid_decimal_format_err(const CompCTX& _ctx, std::ostream& _os) -> void
{
    program_line_err(_ctx, INVALID_DECIMAL_FORMAT, _os);
}

auto invalid_toplevel_expr(const CompCTX& _ctx, std::ostream& _os) -> void
{
    program_line_err(_ctx, INVALID_TOPLEVEL_EXPR, _os);
}

auto unmatched_open_paren(const CompCTX& _ctx, std::ostream& _os) -> void
{
    program_line_err(_ctx, UNMATCHED_OPEN_PAREN, _os);
}

auto unmatched_close_paren(const CompCTX& _ctx, std::ostream& _os) -> void
{
    program_line_err(_ctx, UNMATCHED_CLOSE_PAREN, _os);
}

auto expected_expression(const CompCTX& _ctx, std::ostream& _os) -> void
{
    program_line_err(_ctx, EXPECTED_EXPRESSION, _os);
}

auto expected_declaration(const CompCTX& _ctx, std::ostream& _os) -> void
{
    program_line_err(_ctx, EXPECTED_DECLARATION, _os);
}

auto expected_eq(const CompCTX& _ctx, std::ostream& _os) -> void
{
    program_line_err(_ctx, EXPECTED_EQ, _os);
}

auto invalid_type_identifier(const CompCTX& _ctx, std::ostream& _os) -> void
{
    program_line_err(_ctx, INVALID_TYPE_IDENTIFIER, _os);
}

auto expected_literal(const CompCTX& _ctx, std::ostream& _os) -> void
{
    program_line_err(_ctx, EXPECTED_LITERAL, _os);
}




} // namespace Logger
