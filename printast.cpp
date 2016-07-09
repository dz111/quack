#include <iostream>

#include "node.h"
#include "printast.h"

void PrintAst::visit(NExpression* node) {
  NodeVisitor::visit(node);
}

void PrintAst::visit(NStatement* node) {
  NodeVisitor::visit(node);
}

void PrintAst::visit(NIdentifier* node) {
  std::cout << "identifier " << node->name;
}

void PrintAst::visit(NInteger* node) {
  std::cout << "integer " << node->value;
}

void PrintAst::visit(NReturnStmt* node) {
  std::cout << "return ";
  visit(node->retval);
  std::cout << "\n";
}

void PrintAst::visit(NBlock* node) {
  std::cout << "block\n";
  for (auto stmt : node->stmts) {
    visit(stmt);
  }
}

void PrintAst::visit(NFuncDef* node) {
  std::cout << "function ";
  visit(node->id);
  std::cout << " returns(";
  if (node->ret_type) {
    visit(node->ret_type);
  } else {
    std::cout << "infer";
  }
  std::cout << ") args:\n";
  if (node->args) {
    for (auto arg : *(node->args)) {
      visit(arg);
    }
  } else {
    std::cout << "none";
  }
  visit(node->block);
  std::cout << "\n";
}

void PrintAst::visit(NVarDef* node) {
  std::cout << "vardef type(";
  if (node->type) {
    visit(node->type);
  } else {
   std::cout << "infer";
  }
  std::cout << ") id(";
  visit(node->id);
  std::cout << ") assign/default(";
  if (node->rval) {
    visit(node->rval);
  } else {
    std::cout << "def init/none";
  }
  std::cout << ")\n";
}
