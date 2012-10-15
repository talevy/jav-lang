#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <vector>
#include <string.h>
#include "Entity.h"
using namespace std;
class Entity;

enum TABLE {
  GLOBAL, // global symbol table storing the classes
  CLS, // local vars and meths for class
  METHOD, // params, locals, code holder
  PARAMS, // params table for meth params
  LOCALS // symbols in the body of a method 
};

class SymTable {
 public:
  int _curOffset;
  int _curSize;
  TABLE type;
  vector<Entity*> table;
  SymTable();
  SymTable(TABLE);
  ~SymTable();
  vector<Entity *> * getEntities();
  void setTypeSize(Entity *);
  void insert(Entity *);
  bool hasName(const char *) const;
  Entity * find(const char *) const;
  void printTable() const;
  int size() const;
};

#endif
