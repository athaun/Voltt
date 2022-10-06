#pragma once

#include "ast.hpp"
#include "tokenizer.hpp"

#include <functional>

namespace Voltt {
namespace Parser {

struct CTX;

auto parse(CTX* _ctx) -> void;

auto parse_toplevel_expr(CTX* _ctx) -> AST::Node*;

auto parse_expr(CTX* _ctx) -> AST::Node*;

auto parse_primary_expr(CTX* _ctx) -> AST::Node*;

auto parse_paren_expr(CTX* _ctx) -> AST::Node*;

auto parse_expr_decl(CTX* _ctx) -> AST::Node*;

auto parse_bin_expr(CTX* _ctx, std::function<AST::Node*(CTX*)> _parse_fn, List<Tok::TokID> _cases) -> AST::Node*;

auto inline parse_multiplicative_expr(CTX* _ctx) -> AST::Node*
{
    return parse_bin_expr(
        _ctx,
        parse_primary_expr,
        { Tok::TokenBinOpMul, Tok::TokenBinOpDiv }
    );
}

auto inline parse_addative_expr(CTX* _ctx) -> AST::Node*
{
    return parse_bin_expr(
        _ctx,
        parse_multiplicative_expr,
        { Tok::TokenBinOpAdd, Tok::TokenBinOpSub }
    );
}

auto parse_proto(CTX* _ctx) -> AST::Node*;

auto parse_proto_arg(CTX* _ctx) -> AST::Node*;
 
auto parse_ty(CTX* _ctx, const bool _fn_type = false) -> AST::Node*;

template<typename RET>
auto parse_lit(CTX* _ctx, const AST::NodeType _ty, std::function<RET(const char*)> _parse_fn) -> AST::Node*;

auto parse_lit_num(CTX* _ctx) -> AST::Node*;

auto parse_lit_dec(CTX* _ctx) -> AST::Node*;

auto parse_ident(CTX* _ctx) -> AST::Node*;

auto next_expecting(CTX* _ctx, Tok::TokID _id) -> const bool;

struct CTX {
    size_t pos{};

    const char* fname;
    const char* contents;

    List<Tok::Token> tok_buf;
    List<AST::Node*> body;

    explicit CTX(Tokenizer::CTX* _t)
     : tok_buf(std::move(_t->tok_buf)), fname(_t->fname), contents(std::move(_t->contents))
    {}

    ~CTX()
    {
        //for ( auto& node : body ) AST::free_node(node);
        std::free(const_cast<char*>(contents));
        contents = nullptr;
    }
};

auto inline curr_t(CTX* _ctx) -> Tok::Token&
{
    return _ctx->tok_buf[_ctx->pos];
}

auto inline next_t(CTX* _ctx) -> const bool
{
    if (_ctx->pos+1 >= _ctx->tok_buf.size) return false;
    ++_ctx->pos;
    return true;
}

auto inline lookahead_id(CTX* _ctx) -> const Tok::TokID
{
    if (_ctx->pos+1 >= _ctx->tok_buf.size) return Tok::TokenEndOfFile;
    return _ctx->tok_buf[_ctx->pos+1].id;
}

auto inline dump_errctx_tok(CTX* _ctx, const Tok::Token& _tok) -> const Logger::CompCTX
{
    return Tok::dump_errctx(_tok, _ctx->contents, _ctx->fname);
}

auto inline dump_errctx(CTX* _ctx) -> const Logger::CompCTX
{
    return dump_errctx_tok(_ctx, curr_t(_ctx));
}

auto inline alloc_node() -> AST::Node*
{
    return static_cast<AST::Node*>(std::malloc(sizeof(AST::Node)));
}

auto inline gen_node(CTX* _ctx, const AST::NodeType _ty) -> AST::Node*
{
    AST::Node* node = alloc_node();
    node->type = _ty;
    node->tok = std::move(curr_t(_ctx));
    return node;
}

template<typename RET>
auto parse_lit(CTX* _ctx, const AST::NodeType _ty, std::function<RET(const char*)> _parse_fn) -> AST::Node*
{
    AST::Node* lit = gen_node(_ctx, _ty);

    char const* lit_raw = Tok::to_str(curr_t(_ctx), _ctx->contents);
    switch (_ty) {
        default: Logger::unhandled_case_err("Invalid literal type to parse", DBCTX);

        case AST::TyLitNum: {
            lit->data.lit_num.val = _parse_fn(lit_raw);
            break;
        }

        case AST::TyLitDec: {
            lit->data.lit_dec.val = _parse_fn(lit_raw);
            break;
        }

    }

    next_t(_ctx); // Consume Literal
    
    std::free(const_cast<char*>(lit_raw));

    return lit;
}

} // namespace Parser
} // namespace Voltt
