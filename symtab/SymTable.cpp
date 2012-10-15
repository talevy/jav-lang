
#include "SymTable.h"
#include <stdio.h>
#include <iostream>
using namespace std;

SymTable::SymTable() {
  type = GLOBAL;
  _curOffset = 0;
  _curSize = 0;
}

SymTable::SymTable(TABLE t){
  type = t;
  _curOffset = 0;
  _curSize = 0;
}

SymTable::~SymTable() {
}

vector<Entity *> *
SymTable::getEntities()
{
  return &table;
}


void 
SymTable :: setTypeSize(Entity * e){
  const char * type = e->getType();
  size_t size = 0;
  if (strcmp(type,"char")==0){
    size = 1;
  }
  else if(strcmp(type,"class")==0 
	  || strcmp(type,"meth")==0
	  || strcmp(type,"params")==0
	  || strcmp(type,"locals")==0
	  || strcmp(type,"code")==0){
    size = 0;
  }
  else /* primitive or pointer */{
    size = 4;
  }
  e->setSize(size);
}

void SymTable::insert(Entity * entity) {
  setTypeSize(entity);
  _curSize += entity->_size;
  // get index of first method in table, insert before it
  vector<Entity *>::iterator pos;
  pos = table.end();
  vector<Entity *>::iterator it;
  for (it=table.begin(); it < table.end(); it++) {
    if(strcmp((*it)->getType(), "meth")==0) {
      pos = it;
    }
  }
  // TODO, set size and offsets
  switch (type){
  case CLS:
    entity->setOffset(_curOffset);
    _curOffset += entity->_size;
    break;
  case LOCALS:
    entity->setOffset(_curOffset);
    _curOffset -= entity->_size;
    break;
  case PARAMS:
    _curOffset += entity->_size;
    entity->setOffset(_curOffset);
    break;
  default:
    ;// do nothing
  }

  table.insert(pos, entity);
}

Entity * SymTable::find(const char * name) const {
  vector<Entity*>::size_type sz = table.size();
  for (unsigned int i=0; i < sz; i++){
    Entity *e = table[i];
    if (strcmp(e->getName(), name)==0) {
      return e;
    }
  }
  return NULL;
}


bool SymTable::hasName(const char * name) const {
  Entity *found = this->find(name);
  return found != NULL;
}


void SymTable::printTable() const {
  //queue for printing
  vector<Entity*> q;

  vector<Entity *>::size_type sz = table.size();
  printf("\t____________________________\n");
  for ( unsigned int i=0; i < sz; i++ ){
    Entity *e = table[i];
    printf("\t|%-8s => %-8s|%-2d|%-2d|\n", e->getName(), e->getType()
	   , e->getSize()
	   , e->getOffset());    
    if (e->getSymTable()!=NULL) {
      q.push_back(e);
    }
  }
  printf("\t\u203E\u203E\u203E\u203E\u203E\u203E\u203E\u203E"
         "\u203E\u203E\u203E\u203E\u203E\u203E\u203E\u203E"
	 "\u203E\u203E\u203E\u203E\u203E\u203E\u203E\u203E"
	 "\u203E\u203E\u203E\u203E\n");

  vector<Entity *>::size_type qs = q.size();
  for (unsigned int i=0; i< qs; i++) {
    printf("Entry for: %s\n", q[i]->getName());
    q[i]->getSymTable()->printTable();
  }
}


int
SymTable :: size() const
{
  return _curSize;
}
