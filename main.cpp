#include <iostream>
#include <system_error>
#include "node.h"
#include "parser.hpp"
#include "printast.h"

unsigned long curr_lineno = 1;
FILE* fin;

extern NBlock* programBlock;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "no input file\n";
    return 1;
  }
  fin = std::fopen(argv[1], "r");
  if (!fin) {
    std::cerr << "failed to open input file\n";
    return 1;
  }
  yyparse();
  std::fclose(fin);

//  std::cout << programBlock << std::endl;

  auto printast = PrintAst {};
  printast.visit(programBlock);

  delete programBlock;
  programBlock = nullptr;
}

