#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_

#include "node.h"

#include <cstdio>
#include <string>
#include <unordered_map>

class SymbolTable {
public:
  std::unordered_map<std::string, std::string>
    current_scope;
  std::string lookup(const std::string& name) {
    return current_scope.at(name);
  }
  void add(const std::string& name, const std::string& type) {
    if (current_scope.find(name) == current_scope.end()) {
      current_scope[name] = type;
    } else {
      std::printf("redeclaration of name %s as type %s\n", name.c_str(), type.c_str());
    }
  }
};

class SemanticPass : public NodeVisitor {
public:
  SymbolTable symbol_table;
  void visit(NExpression* node) override;
  void visit(NStatement* node) override;
  void visit(NIdentifier* node) override;
  void visit(NInteger* node) override;
  void visit(NReturnStmt* node) override;
  void visit(NBlock* node) override;
  void visit(NFuncDef* node) override;
  void visit(NVarDef* node) override;
};

#endif  // _SEMANTIC_H_
