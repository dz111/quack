#ifndef _PRINTAST_H_
#define _PRINTAST_H_

#include "node.h"

class PrintAst : public NodeVisitor {
public:
  void visit(NExpression* node) override;
  void visit(NStatement* node) override;
  void visit(NIdentifier* node) override;
  void visit(NInteger* node) override;
  void visit(NReturnStmt* node) override;
  void visit(NBlock* node) override;
  void visit(NFuncDef* node) override;
  void visit(NVarDef* node) override;
};

#endif  // _PRINTAST_H_
