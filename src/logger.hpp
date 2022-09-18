#pragma once

#include <iostream>

namespace Logger {

enum DBGErrno : uint32_t {
    DBG_UNKNOWN_ERR,
    UNREACHABLE_ERR,
    UNHANDLED_CASE_ERR,
    TOKID_STR_ERR,
    TODO_ERR,
};

auto operator<<(std::ostream& _os, const DBGErrno _err) -> std::ostream&;
auto errno_str(const DBGErrno _err) -> const char*;

enum CMPErrno : uint32_t {
    CMP_UNKNOWN_ERR, 
    INVALID_FILE_ERR,
    INVALID_FILE_EXT_ERR,
    READ_FILE_ERR,
    INVALID_DECIMAL_FORMAT,
    INVALID_TOPLEVEL_EXPR,
    UNMATCHED_OPEN_PAREN,
    UNMATCHED_CLOSE_PAREN,
    EXPECTED_EXPRESSION,
    EXPECTED_DECLARATION,
    EXPECTED_EQ,
    INVALID_TYPE_IDENTIFIER,
    EXPECTED_LITERAL,
};

auto operator<<(std::ostream& _os, const CMPErrno _err) -> std::ostream&;
auto errno_str(const CMPErrno _err) -> const char*;

struct DebugCTX {
    const char* fname;
    const char* func;
    const uint32_t line = 0;
};

#define DBCTX { .fname = __FILE__, .func = __func__, .line = __LINE__ }
#define DBCTX_L Logger::DebugCTX

auto operator<<(std::ostream& _os, const DebugCTX& _ctx) -> std::ostream&;

struct CompCTX {
    const size_t start = 0;
    const size_t end = 0;
    const size_t line_start = 0;
    const size_t line_end = 0;
    const size_t line = 0;
    const char* fname;
    const char* fd_contents;
};

auto operator<<(std::ostream& _os, const CompCTX& _ctx) -> std::ostream&;
auto output_prog_line(const CompCTX& _ctx, std::ostream& _os) -> void;

template<typename MSG>
auto inline msg(MSG _msg, std::ostream& _os = std::cerr)
{
    std::cout << _msg << '\n';
}

auto inline dbg_msg(std::ostream& _os = std::cerr) -> std::ostream&
{
    _os << "\n!!! This is a compiler debug error used for compiler development !!!\n";
	_os << "!!! If you are not developing the Voltt compiler this is most likely a bug !!!\n\n";
    return _os;
}

auto inline gen_err(const char* _msg, const DebugCTX& _ctx, const DBGErrno _err, std::ostream& _os = std::cerr) -> void
{
    dbg_msg(_os);

    _os << _err << '\n';
    _os << _ctx << "|\n";
    _os << "| " << _msg << '\n';
    _os << "|\n";

    std::exit(1);
}

auto inline gen_err(const char* _msg, const CompCTX& _ctx, const CMPErrno _err, std::ostream& _os = std::cerr) -> void
{
    _os << _err << '\n';
    _os << _ctx << "|\n";
    _os << "| " << _msg << '\n';
    _os << "|\n";

    std::exit(1);
}

auto inline program_line_err(const CompCTX& _ctx, const CMPErrno _err, std::ostream& _os = std::cerr)
{
    _os << _err << '\n';
    _os << _ctx;
    output_prog_line(_ctx, _os);
    
    std::exit(1);
}

/*
 * Debug ERRS
*/
auto unknown_err(const DebugCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto unreachable_err(const DebugCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto unhandled_case_err(const char* _msg, const DebugCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto tokid_str_err(const DebugCTX& _ctx, std::ostream& _os = std::cerr) -> void; 

/*
 * Comptime ERRS
*/
auto unknown_err(const CompCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto invalid_file_err(const CompCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto invalid_file_extension_err(const CompCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto read_file_err(const CompCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto invalid_decimal_format_err(const CompCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto invalid_toplevel_expr(const CompCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto unmatched_open_paren(const CompCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto unmatched_close_paren(const CompCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto expected_expression(const CompCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto expected_declaration(const CompCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto expected_eq(const CompCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto invalid_type_identifier(const CompCTX& _ctx, std::ostream& _os = std::cerr) -> void;
auto expected_literal(const CompCTX& _ctx, std::ostream& _os = std::cerr) -> void;


} // namespace Logger