#include "ast.hpp"
#include "tok.hpp"
#include <cstdlib>

namespace Voltt {
namespace AST {

auto free_node(Node* _node) -> void
{
    if (_node == nullptr) return;
    switch (_node->type) {
        default: Logger::unhandled_case_err("Invalid Node to free", DBCTX);

        case TyExprDecl: {
            free_node(_node->data.expr_decl.ident);
            free_node(_node->data.expr_decl.type);
            free_node(_node->data.expr_decl.expr);
            goto fn_free_node_done;
        }

        case TyExprBin: {
            free_node(_node->data.expr_bin.lhs);
            free_node(_node->data.expr_bin.rhs);
            goto fn_free_node_done;
        }

        case TyType: {
            AST::Node* ty = _node;
            do {
                free_node(ty->data.ty.ident);
                ty = ty->data.ty.next;
            } while (ty != nullptr);
            goto fn_free_node_done;
        }

        case TyLitNum:
        case TyLitDec: goto fn_free_node_done;
    
        case TyIdent: {
            switch (_node->tok.id) {
                default: break;
                case ALLOC_STR_CASE: std::free(const_cast<char*>(_node->data.ident.raw));
            }
            goto fn_free_node_done;
        }
    }

    fn_free_node_done: {
        std::free(_node);
        return;
    }
}

auto dump(std::ostream& _os, Node* _node, const size_t _level) -> void
{
    switch (_node->type) {
        default: Logger::unhandled_case_err("Invalid ASTNode to traverse", DBCTX);

        case TyExprDecl: {
			dump_indent(_os, _level);
			_os << " └─| Variable\n";

			dump_indent(_os, _level+1);
			_os << " └─| Const\n";

			dump_indent(_os, _level+2);
			_os << " └─> bool: " << _node->data.expr_decl.mut << '\n'; 

			dump(_os, _node->data.expr_decl.ident, _level+1);

			dump(_os, _node->data.expr_decl.type, _level+1);
			
			dump(_os, _node->data.expr_decl.expr, _level+1);

            return;
        }

        case TyExprBin: {
            dump_indent(_os, _level);
            _os << " └─| Binary Expression\n";
            dump_indent(_os, _level+1);

			_os << " └─> op: " << std::to_string(_node->data.expr_bin.op) << '\n';
            
            dump(_os , _node->data.expr_bin.lhs, _level+2);
            dump(_os, _node->data.expr_bin.rhs, _level+2);

            return;
        }

        case TyType: {
            dump_indent(_os, _level);
            _os << " └─| Type\n";

            AST::Node* ty = _node;
            do {
                dump(_os, ty->data.ty.ident, _level+1);
                ty = ty->data.ty.next;
            } while (ty != nullptr);

            return;
        }

        case TyLitNum: {
            dump_indent(_os, _level);
            _os << " └─| Literal Numeric\n";

            dump_indent(_os, _level+1);
			_os << " └─> value: " << _node->data.lit_num.val << '\n';

			return;
        }

        case TyLitDec: {
            dump_indent(_os, _level);
			_os << " └─| Literal Decimal\n";

			dump_indent(_os, _level+1);
			_os << " └─> value: " << _node->data.lit_dec.val << '\n';

			return;
        }

        case TyIdent: {
			dump_indent(_os, _level);
			_os << " └─| Ident\n";

			dump_indent(_os, _level+1);
			_os << " └─> raw: " << _node->data.ident.raw << '\n';

			return;
        }
    }
}

} // namespace AST
} // namespace Voltt
