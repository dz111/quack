#ifndef _NODE_H_
#define _NODE_H_

#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>

#ifndef NOT_REACHED
#define NOT_REACHED std:fprintf(stderr, "NOT_REACHED encountered at %s:%i\n", \
                      __FILE__, __LINE__); \
                    std::abort()
#endif  // NOT_REACHED

enum NExprType {
  TIdentifier,
  TInteger,
  TFloat
};

enum NStmtType {
  TReturnStmt,
  TFuncDef,
  TVarDef
};

class Node {
public:
  virtual ~Node() = default;
};

class NExpression;
class NStatement;
class NVarDef;

using ExpressionList = std::vector<NExpression*>;
using StatementList = std::vector<NStatement*>;
using VarDefList = std::vector<NVarDef*>;

class NExpression : public Node {
public:
  NExprType expr_type;
  NExpression(NExprType expr_type) : expr_type(expr_type) { }
};

class NIdentifier : public NExpression {
public:
  std::string name;
  NIdentifier(const std::string& name) : NExpression(TIdentifier), name(name) { }
};

class NInteger : public NExpression {
public:
  long long value;
  NInteger(long long value) : NExpression(TInteger), value(value) { }
  NInteger(const std::string& value) : NInteger(strtoll(value.c_str(), nullptr, 10)) { }
};

class NStatement : public Node {
public:
  NStmtType stmt_type;
  NStatement(NStmtType stmt_type) : stmt_type(stmt_type) { }
};

class NReturnStmt : public NStatement {
public:
  NExpression* retval;
  NReturnStmt(NExpression* retval) : NStatement(TReturnStmt), retval(retval) { }
  ~NReturnStmt() override { delete retval; }
};

class NBlock : public Node {
public:
  StatementList stmts;
  NBlock() { }
};

class NFuncDef : public NStatement {
public:
  NIdentifier* id;
  NIdentifier* ret_type;
  VarDefList* args;
  NBlock* block;
  NFuncDef(NIdentifier* id, NBlock* block) :
    NFuncDef(id, nullptr, nullptr, block) { }
  NFuncDef(NIdentifier* id, NIdentifier* ret_type, NBlock* block) :
    NFuncDef(id, ret_type, nullptr, block) { }
  NFuncDef(NIdentifier* id, VarDefList* args, NBlock* block) :
    NFuncDef(id, nullptr, args, block) { }
  NFuncDef(NIdentifier* id, NIdentifier* ret_type, VarDefList* args, NBlock* block) :
    NStatement(TFuncDef), id(id), ret_type(ret_type), args(args), block(block) { }
  ~NFuncDef() override { delete id; delete ret_type; delete args; delete block; }
};

class NVarDef : public NStatement {
public:
  NIdentifier* id;
  NIdentifier* type;
  NExpression* rval;
  NVarDef(NIdentifier* id, NIdentifier* type) :
    NVarDef(id, type, nullptr) { }
  NVarDef(NIdentifier* id, NExpression* rval) :
    NVarDef(id, nullptr, rval) { }
  NVarDef(NIdentifier* id, NIdentifier* type, NExpression* rval) :
    NStatement(TVarDef), id(id), type(type), rval(rval) { }
  ~NVarDef() override { delete type; delete id; delete rval; }
};

#define DISPATCH(NODETYPE) \
  case T ## NODETYPE: \
    visit(static_cast<N ## NODETYPE*>(node)); \
    break

class NodeVisitor {
public:
  virtual ~NodeVisitor() = default;
  virtual void visit(NExpression* node) {
    switch (node->expr_type) {
      DISPATCH(Identifier);
      DISPATCH(Integer);
    default:
      NOT_REACHED;
    }
  }
  virtual void visit(NStatement* node) {
    switch (node->stmt_type) {
      DISPATCH(ReturnStmt);
      DISPATCH(FuncDef);
      DISPATCH(VarDef);
    default:
      NOT_REACHED;
    }
  }
  virtual void visit(NIdentifier*) = 0;
  virtual void visit(NInteger*) = 0;
  virtual void visit(NReturnStmt*) = 0;
  virtual void visit(NBlock*) = 0;
  virtual void visit(NFuncDef*) = 0;
  virtual void visit(NVarDef*) = 0;
};

#endif  // _NODE_H_

