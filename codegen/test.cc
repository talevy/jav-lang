#include <string.h>
#include <vector>
#include <stdio.h>
#include <assert.h>
#include "codegen.hh"
using namespace std;
/*
 *
 * Test cases for the codegen
 *
 */

void test()
{
  PTNode p;
  GenCode(&p);
}

int main(int argc, char *argv[]) {
  test(); printf("test passed\n");

}
