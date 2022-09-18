#pragma once

#include "tok.hpp"
#include "../logger.hpp"
#include "../defer.hpp"
#include "../list.hpp"
#include "../todo.hpp"

#include <cstring>
#include <cstdio>

namespace Voltt {
namespace Tokenizer {

#define WHITESPACE_CASE \
	' ': \
	case '\t': \
	case '\r' \

#define NEWLINE_CASE \
	'\n'

#define DIGIT_CASE \
	'0': \
	case '1': \
	case '2': \
	case '3': \
	case '4': \
	case '5': \
	case '6': \
	case '7': \
	case '8': \
	case '9'

#define ALPHA_LOWER_CASE \
	'a': \
	case 'b': \
	case 'c': \
	case 'd': \
	case 'e': \
	case 'f': \
	case 'g': \
	case 'h': \
	case 'i': \
	case 'j': \
	case 'k': \
	case 'l': \
	case 'm': \
	case 'n': \
	case 'o': \
	case 'p': \
	case 'q': \
	case 'r': \
	case 's': \
	case 't': \
	case 'u': \
	case 'v': \
	case 'w': \
	case 'x': \
	case 'y': \
	case 'z'

#define ALPHA_UPPER_CASE \
	'A': \
	case 'B': \
	case 'C': \
	case 'D': \
	case 'E': \
	case 'F': \
	case 'G': \
	case 'H': \
	case 'I': \
	case 'J': \
	case 'K': \
	case 'L': \
	case 'M': \
	case 'N': \
	case 'O': \
	case 'P': \
	case 'Q': \
	case 'R': \
	case 'S': \
	case 'T': \
	case 'U': \
	case 'V': \
	case 'W': \
	case 'X': \
	case 'Y': \
	case 'Z'

#define ALNUM_LOWER_CASE \
	DIGIT_CASE: \
	case ALPHA_LOWER_CASE

#define ALNUM_UPPER_CASE \
	DIGIT_CASE: \
	case ALPHA_UPPER_CASE

#define IDENT_ALLOWED_CASE \
	'_': \
	case DIGIT_CASE: \
	case ALPHA_LOWER_CASE: \
	case ALPHA_UPPER_CASE \

struct VolttKeyword {
	const char* str;
	const Tok::TokID id;
};

enum State {
	STATE_START,
	STATE_IDENT,
	
	STATE_COLON,
	STATE_EQ,
	STATE_CMP_GREATER,
	STATE_CMP_LESS,
	STATE_DASH,

	STATE_LITERAL_NUM,

	STATE_EOF,
};

auto is_valid_extension(const char*) -> const bool;

struct CTX;

auto vlt_keyword_tok(CTX* _ctx, const size_t _start, const size_t _end) -> const Tok::TokID;
auto next_c(CTX* _ctx) -> void;
auto gen_t(CTX* _ctx, const Tok::TokID _id) -> void;
auto next_t(CTX* _ctx) -> void;
auto tokenize(CTX* _ctx) -> void;

auto dump_errctx(const CTX* _ctx) -> const Logger::CompCTX;

struct CTX {
	State state = STATE_START;

	size_t contents_size;
	size_t pos = 0;
	size_t line = 1;
	size_t col = 1;

	List<Tok::Token> tok_buf;
	const char* fname;
	char* contents;

	explicit CTX(const char* _fname)
	: fname(_fname)
	{
		if (!is_valid_extension(_fname)) Logger::invalid_file_extension_err(dump_errctx(this));

		FILE* fd = std::fopen(_fname, "r");
		if (!fd) Logger::invalid_file_err(dump_errctx(this));

		std::fseek(fd, 0, SEEK_END);
		if (std::ferror(fd) != 0) Logger::read_file_err(dump_errctx(this));
									 
		contents_size = std::ftell(fd)+1;
		if (contents_size <= 1) Logger::invalid_file_err(dump_errctx(this));

		contents = static_cast<char*>(std::malloc(contents_size));

		std::fseek(fd, 0, SEEK_SET);
		if (std::ferror(fd) != 0) Logger::read_file_err(dump_errctx(this));
		
		std::fread(contents, contents_size, 1, fd);
		if (ferror(fd) != 0) Logger::read_file_err(dump_errctx(this));
		// I get undefined behavior unless I do this, I thought fread would take care of null termination
		contents[contents_size] = 0;

		std::fclose(fd);
		
		// helps save on realloc calls.
		// contents_size/4 is not the most efficient size, just an estimation
		tok_buf.resize(contents_size >> 2);
	}

	//~CTX() {
		// because tok_buf and contents need to be used during parsing
		// they cannot be freed yet.
		// if they aren't freed at some stage during parsing, yell at me in a PR
	//}
};

auto inline dump_errctx(const CTX* _ctx, const Tok::Token& _tok) -> const Logger::CompCTX
{
	return Tok::dump_errctx(_tok, _ctx->contents, _ctx->fname);
}

} // namespace Tokenizer
} // namespace Voltt
