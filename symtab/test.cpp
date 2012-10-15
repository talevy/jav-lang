#include "ScopeStack.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
using namespace std;

void test_ClassSymbolTableInsertion() {
  /* class hello {} */
  SymTable global;
  Entity s("hello", "int");
  global.insert(&s);
  assert(global.find("hello")==&s);
}

void test_SymbolTableHasName() {
  SymTable global;
  Entity s("hello", "int");
  global.insert(&s);
  assert(global.hasName("hello"));
  assert(!global.hasName("yes"));
}

void test_ScopeStackInsert() {
  SymTable t;
  Entity s("a", "class");
  t.insert(&s);
  ScopeStack stack;
  stack.push(&t);
  assert(stack.getEntityByName("a")==&s);
}

void test_code()
{
  /*
   *
   * class Test {
   *  int a, b;
   *  int fun(int c) {
   *     a = c;
   *     b = 2*c;
   *  }
   * }
   *
   */
  SymTable global;
  Entity cls_test("Test", "class");
  SymTable *classTable = cls_test.getSymTable();
  global.insert(&cls_test);
  Entity a("a", "int");
  Entity b("b", "int");
  Entity meth_fun("fun", "meth");
  Entity c("c", "int");
  classTable->insert(&a);
  classTable->insert(&b);
  classTable->insert(&meth_fun);
  SymTable *methTable = meth_fun.getSymTable();
  methTable->find("params")->getSymTable()->insert(&c);
  assert(true);
  global.printTable();
  //TODO
}


void test_offset()
{
  SymTable global;
  Entity A("A", "class");
  SymTable *A_tab = A.getSymTable();
  global.insert(&A);
  Entity a("a", "int"), b("b", "char"),
    c("c", "int"), d("d", "char");
  Entity meth("func", "meth");
  A_tab->insert(&a);
  A_tab->insert(&b);
  A_tab->insert(&meth);
  SymTable *meth_tab = meth.getSymTable();
  SymTable *p_tab = meth_tab->find("params")->getSymTable();
  p_tab->insert(&c);
  p_tab->insert(&d);
  
}

int main(int argc, char *argv[]) {
  test_ClassSymbolTableInsertion();
  printf("test_ClassSymbolTableInsertion passed\n");
  test_SymbolTableHasName();
  printf("test_SymbolTableHasName() passed\n");
  test_ScopeStackInsert();
  printf("test_ScopeStackInsert passed\n");
  test_code();
  printf("test_code passed\n");
  test_offset();
  printf("test_offset passed\n");
}
