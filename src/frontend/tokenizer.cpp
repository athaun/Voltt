#include "tokenizer.hpp"
#include "tok.hpp"

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
	{"s32", Tok::TokenTypS32},
	{"fn", Tok::TokenFn},
};

auto vlt_keyword_tok(const CTX* _ctx, const size_t _start, const size_t _end) -> const Tok::TokID
{
	const size_t len = (_end-_start);

	for ( size_t i = 0; i < ARR_LEN(VLT_KEYWORDS); i++ ) {
		if (len+1 != std::strlen(VLT_KEYWORDS[i].str)) continue;
		if (std::strncmp(_ctx->contents+_start, VLT_KEYWORDS[i].str, strlen(VLT_KEYWORDS[i].str)) == 0) return VLT_KEYWORDS[i].id;
	}
	
	bool is_decimal = false;
	for ( size_t idx = 0; idx <= len; idx++) {
		switch (_ctx->contents[_start+idx]) {
			default: return Tok::TokenIdent;

			case DIGIT_CASE: break;

			case '.':
				if (is_decimal) { 
					Logger::cmperr(
						Tok::dump_errctx(
							_ctx->tok_buf.back(),
							_ctx->contents,
							_ctx->fname
						),
						Logger::CompErrID::INVALID_DECIMAL_FORMAT
					);
				}

				is_decimal = true;
				break;
		}
	}

	return (is_decimal) ? Tok::TokenLiteralDecimal : Tok::TokenLiteralNumeric;
}

auto next_c(CTX* _ctx) -> void 
{
	if (++_ctx->pos >= _ctx->contents_size) {
		_ctx->state = STATE_EOF;
		return;
	}

	if (_ctx->contents[_ctx->pos] == 10) {
		++_ctx->line;
		_ctx->col = 0;
	}
	else ++_ctx->col;
}

auto gen_t(CTX* _ctx, const Tok::TokID _id) -> void
{
	_ctx->tok_buf.emplace_back(
		_id,
		_ctx->pos,
		_ctx->line,
		_ctx->col
	);
}

auto next_t(CTX* _ctx) -> void
{
	switch (_ctx->state) {
		default: Logger::unhandled_case_err("Failed to handle all the cases during tokenization states");

		case STATE_EOF:
			goto done;

		case STATE_START:
			switch(_ctx->contents[_ctx->pos]) {
				default: Logger::unhandled_case_err("Failed to handle all cases during tokenization");

				case 0:
				case WHITESPACE_CASE: goto done;

				case NEWLINE_CASE:
					gen_t(_ctx, Tok::TokenEndStatement);
					goto done;

				case ':':
					gen_t(_ctx, Tok::TokenColonSymbol);
					_ctx->state = STATE_COLON;
					goto done;

				case '=':
					gen_t(_ctx, Tok::TokenEqSymbol);
					goto done;

				case '>':
					gen_t(_ctx, Tok::TokenCmpGreater);
					_ctx->state = STATE_CMP_GREATER;
					goto done;

				case '<':
					gen_t(_ctx, Tok::TokenCmpLess);
					_ctx->state = STATE_CMP_LESS;
					goto done;

				case ',':
					gen_t(_ctx, Tok::TokenComma);
					goto done;

				case '(':
					gen_t(_ctx, Tok::TokenParenOpen);
					goto done;

				case ')':
					gen_t(_ctx, Tok::TokenParenClose);
					goto done;

				case '{':
					gen_t(_ctx, Tok::TokenBracketOpen);
					goto done;

				case '}':
					gen_t(_ctx, Tok::TokenBracketClose);
					goto done;

				case '+':
					gen_t(_ctx, Tok::TokenBinOpAdd);
					goto done;

				case '-':
					gen_t(_ctx, Tok::TokenBinOpSub);
					_ctx->state = STATE_DASH;

					goto done;

				case '*':
					gen_t(_ctx, Tok::TokenBinOpMul);
					goto done;

				case '/':
					gen_t(_ctx, Tok::TokenBinOpDiv);
					goto done;

				case IDENT_ALLOWED_CASE:
					gen_t(_ctx, Tok::TokenIdent);
					_ctx->state = STATE_IDENT;
					goto done;
			}

		case STATE_CMP_GREATER:
			switch(_ctx->contents[_ctx->pos]) {
				default:
					_ctx->state = STATE_START;
					goto skip_iter;

			case '=':
				_ctx->tok_buf.back().id = Tok::TokenCmpGreaterEq;
				_ctx->tok_buf.back().end = _ctx->pos;

				_ctx->state = STATE_START;
				goto done;
			
			case '-':
				_ctx->tok_buf.back().id = Tok::TokenArrowLeft;
				_ctx->tok_buf.back().end = _ctx->pos;

				_ctx->state = STATE_START;
				goto done;
			}

		case STATE_CMP_LESS:
			switch(_ctx->contents[_ctx->pos]) {
				default:
					_ctx->state = STATE_START;
					goto skip_iter;
			
				case '=':
					_ctx->tok_buf.back().id = Tok::TokenCmpLessEq;
					_ctx->tok_buf.back().end = _ctx->pos;

					_ctx->state = STATE_START;
					goto done;
			}

		case STATE_DASH:
			switch(_ctx->contents[_ctx->pos]) {
				default:
					_ctx->state = STATE_START;
					goto skip_iter;

				case '>':
					_ctx->tok_buf.back().id = Tok::TokenArrowRight;
					_ctx->tok_buf.back().end = _ctx->pos;

					_ctx->state = STATE_START;
					goto done;
			}

		case STATE_COLON:
			switch(_ctx->contents[_ctx->pos]) {
				default: 
					_ctx->state = STATE_START;
					goto skip_iter;

				case ':':
					_ctx->tok_buf.back().id = Tok::TokenColonInferConst;
					_ctx->tok_buf.back().end = _ctx->pos;
					
					_ctx->state = STATE_START;
					goto done;

				case '=':
					_ctx->tok_buf.back().id = Tok::TokenColonInferConst;
					_ctx->tok_buf.back().end = _ctx->pos;
					
					_ctx->state = STATE_START;
					goto done;
			}

		case STATE_IDENT: // handles idents, keywords, and literals 
			for (;;) {
			switch(_ctx->contents[_ctx->pos]) {
				case 0:
				default:
					_ctx->tok_buf.back().id = vlt_keyword_tok(
						_ctx,
						_ctx->tok_buf.back().offset,
						_ctx->pos-1
					);
					_ctx->tok_buf.back().end = _ctx->pos-1;
					_ctx->state = STATE_START;
					goto skip_iter;

				case IDENT_ALLOWED_CASE:
				case '.':
					next_c(_ctx);
					continue;
			}
			}
	}
	
	done: next_c(_ctx); // dear god, goto... in 2022??
	skip_iter: return;
}

auto tokenize(CTX* _ctx) -> void
{
	while ( _ctx->state != STATE_EOF ) next_t(_ctx);
	_ctx->tok_buf.emplace_back(
		Tok::TokenEndOfFile,
		_ctx->pos+1,
		_ctx->line,
		_ctx->col+1
	);
}

} // namespace Tokenizer
} // namespace Voltt
