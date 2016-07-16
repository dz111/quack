#include <cstdio>

#include "node.h"
#include "semantic.h"

void SemanticPass::visit(NExpression* node) {
  NodeVisitor::visit(node);
}

void SemanticPass::visit(NStatement* node) {
  NodeVisitor::visit(node);
}

void SemanticPass::visit(NIdentifier* node) {
  node->value_type = symbol_table.lookup(node->name);
  std::printf("inspecting identifier node name=%s type=%s\n", node->name.c_str(), node->value_type.c_str());
}

void SemanticPass::visit(NInteger* node) {
  node->value_type = "int";
  std::printf("inspecting integer node value=%lld\n", node->value);
}

void SemanticPass::visit(NReturnStmt* node) {
  visit(node->retval);
  std::printf("inspecting return statement type=%s\n", node->retval->value_type.c_str());
}

void SemanticPass::visit(NBlock* node) {
  for (auto stmt : node->stmts) {
    visit(stmt);
  }
}

void SemanticPass::visit(NFuncDef* node) {
  symbol_table.add(node->id->name, node->ret_type->name.c_str());
  std::printf("inspecting function name=%s rettype=%s\n", node->id->name.c_str(), node->ret_type->name.c_str());
  for (auto arg : *(node->args)) {
    std::string type;
    if (arg->type) {
      type = arg->type->name;
    } else if (arg->rval) {
      type = arg->rval->value_type;
    } else {
      std::fprintf(stderr, "In definition of function %s, could not deduce type of parameter %s\n",
                   node->id->name.c_str(), arg->id->name.c_str());
    }
    symbol_table.add(arg->id->name, type);
    std::printf("in function %s, inspecting argument name=%s type=%s\n", node->id->name.c_str(), arg->id->name.c_str(), type.c_str());
    if (arg->rval && arg->rval->value_type != type) {
      std::fprintf(stderr, "In definition of function %s, parameter %s (type %s)"
                           " did not match default value (type %s)\n",
                   node->id->name.c_str(), arg->id->name.c_str(), type.c_str(), arg->rval->value_type.c_str());
    }
  }
  visit(node->block);
}

void SemanticPass::visit(NVarDef* node) {
  
}
