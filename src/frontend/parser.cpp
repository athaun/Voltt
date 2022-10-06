#include "parser.hpp"
#include "ast.hpp"
#include "tok.hpp"

namespace Voltt {
namespace Parser {

auto next_expecting(CTX *_ctx, Tok::TokID _id) -> const bool
{
    if (_ctx->pos+1 >= _ctx->tok_buf.size) return false;
    if (_ctx->tok_buf[_ctx->pos+1].id != _id) return false;
    ++_ctx->pos;
    return true;
}

auto parse(CTX* _ctx) -> void
{
    for (;;) {
        if (curr_t(_ctx).id == Tok::TokenEndOfFile) return;
        
        AST::Node* node = parse_toplevel_expr(_ctx);

        if (node == nullptr) {
            return;
        
        } else if (curr_t(_ctx).id == Tok::TokenEndStatement) {
            next_t(_ctx); // Consume EndOfStatement

        } else if (curr_t(_ctx).id == Tok::TokenParenClose) {
            Logger::msg("MADE IT");
            Logger::unmatched_close_paren(dump_errctx(_ctx));  
        
        } _ctx->body.push(std::move(node));
    }
}

auto parse_toplevel_expr(CTX* _ctx) -> AST::Node*
{
    switch (curr_t(_ctx).id) {
        default: Logger::invalid_toplevel_expr(dump_errctx(_ctx));

        case Tok::TokenIdent: return parse_expr_decl(_ctx);
        
        case Tok::TokenEndStatement:
        case Tok::TokenEndOfFile: return nullptr;
    }
}

auto parse_expr(CTX* _ctx) -> AST::Node*
{
    return parse_addative_expr(_ctx);
}

auto parse_primary_expr(CTX* _ctx) -> AST::Node*
{
    switch (curr_t(_ctx).id) {
        default: Logger::expected_expression(dump_errctx(_ctx));

        case Tok::TokenParenOpen: return parse_paren_expr(_ctx);
        case Tok::TokenIdent: return parse_ident(_ctx);
        case Tok::TokenLiteralNumeric: return parse_lit_num(_ctx);
        case Tok::TokenLiteralDecimal: return parse_lit_dec(_ctx);
    }
}

auto parse_paren_expr(CTX* _ctx) -> AST::Node*
{
    if (next_expecting(_ctx, Tok::TokenParenClose)) { // empty paren case
        next_t(_ctx); // Consume ')'
        return parse_expr(_ctx);
    }
    
    next_t(_ctx); // Consume '('
    AST::Node* expr = parse_expr(_ctx); // filled paren case
    if (curr_t(_ctx).id != Tok::TokenParenClose) Logger::unmatched_close_paren(dump_errctx(_ctx));
    next_t(_ctx); // Consume ')'
    return expr;
}

auto parse_expr_decl(CTX* _ctx) -> AST::Node*
{
    AST::Node* var = gen_node(_ctx, AST::TyExprDecl);
    var->data.expr_decl.ident = parse_ident(_ctx);

    switch (curr_t(_ctx).id) {
        default: Logger::expected_declaration(dump_errctx(_ctx));

        case Tok::TokenColonSymbol: {
            next_t(_ctx); // Consume ':'
            var->data.expr_decl.type = parse_ty(_ctx);

            if (curr_t(_ctx).id == Tok::TokenColonSymbol) {
                var->data.expr_decl.mut = false;

            } else if (curr_t(_ctx).id == Tok::TokenEqSymbol) {
                var->data.expr_decl.mut = true;

            } else Logger::expected_eq(dump_errctx(_ctx));

            break;
        }

        case Tok::TokenColonInferMut: {
            var->data.expr_decl.mut = true;   
            break;
        }
        case Tok::TokenColonInferConst: {
            var->data.expr_decl.mut = false;
            break;
        }
    }

    next_t(_ctx); // Consume ':' <|> '=' <|> '::' <|> ':='
    var->data.expr_decl.expr = parse_expr(_ctx);

    return var;
}

auto parse_bin_expr(CTX* _ctx, std::function<AST::Node*(CTX*)> _parse_fn, List<Tok::TokID> _cases) -> AST::Node*
{
    TODO
    AST::Node* lhs = _parse_fn(_ctx);
    for (;;) {
        const Tok::Token op = curr_t(_ctx);

        for ( const auto& id : _cases ) if (op.id == id) goto fn_parse_bin_expr_case_end;
        goto fn_parse_bin_expr_end;

        fn_parse_bin_expr_case_end: {    
            next_t(_ctx); // Consume OP

            AST::Node* rhs = _parse_fn(_ctx);
            AST::Node* new_lhs = gen_node(_ctx, AST::TyExprBin);
            new_lhs->data.expr_bin.op = op.id;
            new_lhs->data.expr_bin.lhs = lhs;
            new_lhs->data.expr_bin.rhs = rhs;
    
            lhs = new_lhs;
        }
    }

    fn_parse_bin_expr_end: return lhs;
}

auto parse_proto(CTX* _ctx) -> AST::Node*
{
    TODO
    if (curr_t(_ctx).id != Tok::TokenParenOpen) Logger::unhandled_case_err("Function prototypes require parenthesis.", DBCTX);

    AST::Node* root_arg = gen_node(_ctx, AST::TyProto);
    root_arg->data.proto.arg = gen_node(_ctx, AST::TyProtoArg);
    AST::Node* arg = root_arg; 

    for (;;) {
        arg->data.proto.arg->data.proto_arg.ident = parse_ident(_ctx);
        if (!next_expecting(_ctx, Tok::TokenColonSymbol)) Logger::unhandled_case_err("Prototype arguments require a type specifier.", DBCTX);
        arg->data.proto.arg->data.proto_arg.type = parse_ty(_ctx, true);

        switch (curr_t(_ctx).id) {
            default: Logger::unhandled_case_err("Uqualified ident", DBCTX);

            case Tok::TokenParenClose: {
                next_t(_ctx); // Consume ')'
                goto fn_parse_proto_end;
            }
            case Tok::TokenComma: {
                Tok::dump(std::cout, curr_t(_ctx), _ctx->contents);
                arg->data.proto.next = gen_node(_ctx, AST::TyProtoArg);
                //arg->data.proto.next->data.proto_arg = gen_node(_ctx, AST::TyProtoArg);
                arg = arg->data.proto.next;
            };
        }

    }

    fn_parse_proto_end: return root_arg;
}

auto parse_proto_arg(CTX* _ctx) -> AST::Node*
{
    TODO
    AST::Node* arg = gen_node(_ctx, AST::TyProtoArg);
    arg->data.proto_arg.ident = parse_ident(_ctx);
    arg->data.proto_arg.type = parse_ty(_ctx);
    return arg;
}

auto parse_ty(CTX* _ctx, const bool _fn_type) -> AST::Node*
{
    switch (curr_t(_ctx).id) {
        default: break;
        
        case Tok::TokenFn: {
            next_t(_ctx); // Consume 'fn'
            return parse_proto(_ctx);
        }

        // case struct
        // case enum
        // case union
    }

    AST::Node* root_ty = gen_node(_ctx, AST::TyType);
    AST::Node* ty = root_ty;

    for (;;) {
        ty->data.ty.ident = parse_ident(_ctx); // parse and consume type
        switch (curr_t(_ctx).id) {
            default: Logger::invalid_type_identifier(dump_errctx(_ctx));

            case Tok::TokenParenClose:
            case Tok::TokenComma: if (!_fn_type) goto fn_parse_ty_end;
            
            case Tok::TokenColonSymbol:
            case Tok::TokenEqSymbol: goto fn_parse_ty_end;

            case VTYPES_CASE:
            case Tok::TokenIdent: {
                ty->data.ty.next = gen_node(_ctx, AST::TyType);
                ty = ty->data.ty.next;
            }
        }
    }

    fn_parse_ty_end: return root_ty;
}

auto parse_lit_num(CTX* _ctx) -> AST::Node*
{
    return parse_lit<int32_t>(_ctx, AST::TyLitNum, atoi);
}

auto parse_lit_dec(CTX* _ctx) -> AST::Node*
{
    return parse_lit<double>(_ctx, AST::TyLitDec, atof);
}

auto parse_ident(CTX* _ctx) -> AST::Node*
{
    AST::Node* ident = gen_node(_ctx, AST::TyIdent);
    ident->data.ident.raw = Tok::to_str(curr_t(_ctx), _ctx->contents);

    next_t(_ctx); // Consume Ident

    return ident;
}

} // namespace Parser
} // namespace Voltt
