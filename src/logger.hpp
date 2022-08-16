#pragma once

#include "todo.hpp"

#include <iostream>
#include <ostream>
#include <cstdint>

namespace Logger {

static std::ostream& _os = std::cerr;

struct DebugCtx_t {
	const char* m_file;
	const char* m_func;
	const uint32_t m_line;
};

#define DBCTX DebugCtx_t{ .m_file = __FILE__, .m_func = __func__, .m_line = __LINE__ }

auto operator<<(std::ostream&, const DebugCtx_t&) -> std::ostream&;

namespace DebugErrID {
enum DebugErrID_t : uint8_t {
	UNKNOWN,
	TODO_ERR,
	UNREACHABLE_ERR,
	UNHANDLED_CASE_ERR,
	TOKID_STR_ERR,
	TOK_STATE_ERR,
	ASTGEN_INVALID_TOK_ERR,
	UNWRAP_ERR,
};

auto to_str(const DebugErrID_t) -> const char*;

} // namespace DebugErrID

struct CompCtx_t {
	const uint32_t m_line_before_start;
	const uint32_t m_line_after_end;
	const char* m_fname;
	const char* m_fd_contents;
};

auto operator<<(std::ostream&, const CompCtx_t&) -> std::ostream&;

namespace CompErrID {
enum CompErrID_t : uint8_t {
	UNKNOWN,
	INVALID_FILE_EXTENSION_ERR,
	UNKNOWN_FILE_ERR,
	READ_FILE_ERR,
	INVALID_TYPE_ERR,
};

auto to_str(const CompErrID_t) -> const char*;

} // namespace CompErrID

template<typename MSG>
auto debug(const DebugCtx_t& _ctx, const DebugErrID::DebugErrID_t _id, MSG _msg) -> void
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

template<typename MSG>
auto cmperr(const CompCtx_t& _ctx, const CompErrID::CompErrID_t _id, MSG _msg) -> void
{
	_os << "ERR![E" << std::to_string(_id) << "]: " << CompErrID::to_str(_id) << '\n';
	_os << _ctx << '\n';

	exit(1);
}

template<typename MSG>
auto msg(const DebugCtx_t& _ctx, MSG _msg) -> void
{
	_os << "MSG![]:\n";
	_os << _ctx << '\n';
	_os << " |\n";
	_os << " | " << _msg << std::endl;
	_os << " |\n";
}

auto read_file_err(const char*) -> void;
auto invalid_extension_err(const char*) -> void;
auto invalid_file_err(const char*) -> void;

#define unreachable_err() debug(Logger::DBCTX, Logger::DebugErrID::UNREACHABLE_ERR, "This wasn't supposed to happen.")
#define unhandled_case_err(msg) debug(Logger::DBCTX, Logger::DebugErrID::UNHANDLED_CASE_ERR, msg)

} // namespace Logger
