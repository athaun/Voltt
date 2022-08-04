#pragma once

#include <iostream>
#include <ostream>
#include <fstream>
#include <memory>
#include <vector>

namespace Logger {

auto flush(std::ostream& _os) -> void;

struct DebugCtx_t {
	const char* m_file;
	const char* m_func;
	const uint32_t m_line;
};

#define DBCTX DebugCtx_t{ .m_file = __FILE__, .m_func = __func__, .m_line = __LINE__+1 }

auto operator<<(std::ostream&, const DebugCtx_t&) -> std::ostream&;

namespace DebugErrID {
enum DebugErrID_t {
	UNKNOWN,
};

auto to_str(const DebugErrID_t) -> const char*;

} // namespace DebugErrID

struct CompCtx_t {
	const char* m_fname;
	const uint32_t m_err_start_pos;
	const uint32_t m_err_end_pos;
	const uint32_t m_err_line;
	const uint32_t m_err_col;
};

auto operator<<(std::ostream&, const CompCtx_t&) -> std::ostream&;

namespace CompErrID {
enum CompErrID_t {
	UNKNOWN,
};

auto to_str(const CompErrID_t) -> const char*;

} // namespace CompErrID

auto debug(const DebugCtx_t& _ctx, const DebugErrID::DebugErrID_t _id, const char* _msg) -> void;
auto cmperr(const CompCtx_t& _ctx, const CompErrID::CompErrID_t _id, const char* _msg) -> void;

} // namespace Logger
