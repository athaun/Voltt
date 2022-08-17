#include "astgen.hpp"

namespace Voltt {
namespace ASTGen {

auto ast_dump_node_indent(std::ostream& _os, size_t _level) -> void
{
	_level *= 2;
	while (_level--) _os.write(" ", 1);
}

auto ast_dump_node(std::ostream& _os, const ASTNode::Node* _node, const size_t _level) -> void
{
	if (_node == nullptr) {
		ast_dump_node_indent(_os, _level+1);
		_os << " └─> Infer\n";
		return;
	} 

	switch (_node->type) {
		default: Logger::unhandled_case_err("Invalid ASTNode to traverse");

		case ASTNode::TypeExprBinary:
			ast_dump_node_indent(_os, _level);
			_os << " └─| Binary Expression\n";

			ast_dump_node_indent(_os, _level+1);
			_os << " └─> op: " << std::to_string(_node->data.expr_binary_data.op) << '\n';

			ast_dump_node(_os, _node->data.expr_binary_data.left, _level+1);
			ast_dump_node(_os, _node->data.expr_binary_data.right, _level+1);

			return;

		case ASTNode::TypeLiteralNumeric:
			
			ast_dump_node_indent(_os, _level);
			_os << " └─| Literal Numeric\n";
			
			ast_dump_node_indent(_os, _level+1);
			_os << " └─> value: " << _node->data.literal_numeric_data.value << '\n';

			return;

		case ASTNode::TypeLiteralDeciamal:
			
			ast_dump_node_indent(_os, _level);
			_os << " └─| Literal Decimal\n";

			ast_dump_node_indent(_os, _level+1);
			_os << " └─> value: " << _node->data.literal_decimal_data.value << '\n';

			return;

		case ASTNode::TypeIdent:

			ast_dump_node_indent(_os, _level);
			_os << " └─| Ident\n";

			ast_dump_node_indent(_os, _level+1);
			_os << " └─> raw: " << _node->data.ident_data.raw << '\n';

			return;

		case ASTNode::TypeTy:
			
			ast_dump_node(_os, _node->data.ty_data.ty, _level);

			return;

		case ASTNode::TypeVariableDecl:
			
			ast_dump_node_indent(_os, _level);
			_os << " └─| Variable\n";
			
			ast_dump_node_indent(_os, _level+1);
			_os << " └─| Const\n";
			ast_dump_node_indent(_os, _level+2);
			_os << " └─> bool: " << _node->data.variable_decl_data.is_const << '\n'; 

			ast_dump_node(_os, _node->data.variable_decl_data.ident, _level+1);
			
			ast_dump_node_indent(_os, _level+1);
			_os << " └─| Type\n";

			ast_dump_node(_os, _node->data.variable_decl_data.type, _level+1);
			
			ast_dump_node(_os, _node->data.variable_decl_data.expr, _level+1);

			return;


	}
	Logger::unreachable_err();
}

} // namespace ASTGen
} // namespace Voltt
