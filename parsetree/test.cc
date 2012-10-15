#include <string.h>
#include <vector>
#include <stdio.h>
#include <assert.h>
#include "PTNodes.hh"
using namespace std;
/*
 *
 * Test cases for the parse tree
 *
 */

void test_tree()
{
  /*Code test:
   *
   * class test {
   *    int a, b
   * }
   *
   */
  PTNode p;
  /*
  LeafNode ln("int");
  LeafNode lna("a");
  LeafNode lnb("b");
  LeafNode cname("test");

  PrimitiveType pt(&ln);
  Type t(&pt);
  VariableDeclarator vd1(&lna);
  VariableDeclarator vd2(&lnb);
  VariableDeclarators vds2(&vd1);
  VariableDeclarators vds1(&vds2, &vd2);

  FieldDeclaration fd(&t, &vds1);
  ClassMemberDeclaration *cd = &fd;
  ClassBodyDeclarations cds(cd);

  ClassBody cb(&cds);
  ClassDeclaration cl(&cname, &cb);
  
  ClassDeclarations cls(&cl);
  Program p(&cls);
  assert(p.getChildren()->at(0) == &cls);
  */
}

int main(int argc, char *argv[]) {
  test_tree(); printf("test_tree passed\n");
}
