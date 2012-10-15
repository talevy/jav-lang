#ifndef ENTITY_H
#define ENTITY_H

#include "SymTable.h"
#include "../parsetree/PTNodes.hh"
#include <string.h>
class SymTable;
class PTNode;

class Entity {
 private:
  PTNode *ptnode;
  SymTable *symtab;
  const char * _codename;
 public:
  int _size;
  int _offset;
  const char * _name;
  const char * _type;
  bool isMethod;
  Entity(const char*, const char *);
  ~Entity();
  void setParseTree(PTNode *);
  PTNode * getParseTree();
  SymTable * getSymTable();
  void setType(const char *);
  const char * getType();
  const char * getName();
  void setOffset(int);
  int getOffset();
  void setSize(int);
  int getSize();
  const char * codeName();
  void setCodeName(const char *);
};

#endif
