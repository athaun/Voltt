
#include "tokenizer.hpp"

namespace Voltt {
namespace Tokenizer {

#define ARR_LEN(x) ((*(&x+1)) - x)

static const char* VLT_EXTENSIONS[] = {
	".vlt",
};

auto is_valid_extension(const char* _fname) -> bool
{
	for ( size_t i = 0; i < ARR_LEN(VLT_EXTENSIONS); i++ ) {
		const char* is_valid = std::strstr(_fname, VLT_EXTENSIONS[i]);
		if ( (is_valid != NULL) && (strlen(is_valid) == strlen(VLT_EXTENSIONS[i])) ) return true;
	}

	return false;
}

static const VolttKeyword VLT_KEYWORDS[] = {
	{"::", Tok::TokenColonInferConst},
	{":=", Tok::TokenColonInferMut},
	{"s32", Tok::TokenTypS32},
};

auto static vlt_keyword_tok(const char* _source, const size_t _start, const size_t _end) -> const Tok::TokID
{
	const size_t len = (_end-_start);
	for ( size_t i = 0; i < ARR_LEN(VLT_KEYWORDS); i++ ) {
		if (len+1 != std::strlen(VLT_KEYWORDS[i].str)) continue;
		if (std::strncmp(_source+_start, VLT_KEYWORDS[i].str, strlen(VLT_KEYWORDS[i].str)) == 0) return VLT_KEYWORDS[i].id;
	}

	bool is_decimal = false;
	for ( size_t i = 0; i < len; i++ ) {
		switch(_source[_start+i]) {
			default: return Tok::TokenIdent;
			case '.':
				if (!is_decimal) {
					is_decimal = true;
					continue;
				}
				Logger::unhandled_case_err("Invalid expression");
			case DIGIT_CASE: continue;
		}
	}
	return (is_decimal) ? Tok::TokenLiteralDecimal : Tok::TokenLiteralNumeric;
}

auto next_c(TokenizerCTX* _t) -> void 
{
	if (++_t->pos >= _t->contents_size) {
		_t->state = STATE_EOF;
		return;
	}

	if (_t->contents[_t->pos] == 10) {
		++_t->line;
		_t->col = 0;
	}
	else ++_t->col;
}

auto gen_t(TokenizerCTX* _t, const Tok::TokID _id) -> void
{
	_t->tok_buf.emplace_back(
		_id,
		_t->pos,
		_t->line,
		_t->col
	);
}

auto next_t(TokenizerCTX* _t) -> void
{
	switch (_t->state) {
		default: Logger::unhandled_case_err("Failed to handle all the cases during tokenization states");

		case STATE_EOF:
			goto done;

		case STATE_START:
			switch(_t->contents[_t->pos]) {
				default: Logger::unhandled_case_err("Failed to handle all cases during tokenization");

				case 0:
				case WHITESPACE_CASE: goto done;

				case NEWLINE_CASE:
					gen_t(_t, Tok::TokenEndStatement);
					goto done;

				case ':':
					gen_t(_t, Tok::TokenColonSymbol);
					_t->state = STATE_COLON;
					goto done;

				case '=':
					gen_t(_t, Tok::TokenEqSymbol);
					goto done;

				case '(':
					gen_t(_t, Tok::TokenParenOpen);
					goto done;

				case ')':
					gen_t(_t, Tok::TokenParenClose);
					goto done;

				case '+':
					gen_t(_t, Tok::TokenBinOpAdd);
					goto done;

				case '-':
					gen_t(_t, Tok::TokenBinOpSub);
					goto done;

				case '*':
					gen_t(_t, Tok::TokenBinOpMul);
					goto done;

				case '/':
					gen_t(_t, Tok::TokenBinOpDiv);
					goto done;

				case IDENT_ALLOWED_CASE:
					gen_t(_t, Tok::TokenIdent);
					_t->state = STATE_IDENT;
					goto done;
			}

		case STATE_COLON:
			switch(_t->contents[_t->pos]) {
				default: 
					_t->state = STATE_START;
					goto skip_iter;

				case ':':
					_t->tok_buf.back().id = vlt_keyword_tok(
						_t->contents,
						_t->tok_buf.back().offset,
						_t->pos
					); 
					_t->state = STATE_START;
					goto done;

				case '=':
					_t->tok_buf.back().id = vlt_keyword_tok(
						_t->contents,
						_t->tok_buf.back().offset,
						_t->pos
					);
					_t->state = STATE_START;
					goto done;
			}

		case STATE_IDENT: // handles idents, keywords, and literals 
			for (;;) {
			switch(_t->contents[_t->pos]) {
				case 0:
				default:
					_t->tok_buf.back().id = vlt_keyword_tok(
						_t->contents,
						_t->tok_buf.back().offset,
						_t->pos-1
					);
					_t->tok_buf.back().end = _t->pos-1;
					_t->state = STATE_START;
					goto skip_iter;

				case IDENT_ALLOWED_CASE:
				case '.':
					next_c(_t);
					continue;
			}
			}
	}
	
	done: next_c(_t); // dear god, goto... in 2022??
	skip_iter: return;
}

auto tokenize(TokenizerCTX* _t) -> void
{
	while ( _t->state != STATE_EOF ) next_t(_t);
	_t->tok_buf.emplace_back(
		Tok::TokenEndOfFile,
		_t->pos+1,
		_t->line,
		_t->col+1
	);
}

} // namespace Tokenizer
} // namespace Voltt
