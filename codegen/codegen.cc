#include "codegen.hh"
#include <stdio.h>
#include <stdlib.h>

// used to store increasing temp var names
static char tvar[] = "t0000000";
static char label[] = "l0000000";
void propagateTypeDown(PTNode * node, const char *type){
  if (node==NULL) { return ; }

  node->_type = type;

  vector<PTNode *> * children = node->getChildren();
  vector<PTNode *>::iterator it;
  for (it = children->begin(); it < children->end(); it++){
    propagateTypeDown(*it, type);
  }
}





void setTypeSize(Entity * e){
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

void setClassSizes(SymTable *symtab){
  if(symtab->type != GLOBAL){
    return;
  }
  vector<Entity *> list = symtab->table;
  vector<Entity *>::iterator it;
  for (it = list.begin(); it< list.end(); it++) {
    setClassSize(*it);
  }

}
void setClassSize(Entity * cls) {
  SymTable * t = cls->getSymTable();
  vector<Entity *> list = t->table;
  vector<Entity *>::iterator it;
  size_t size = 0;
  if (t!=NULL){
    for (it = list.begin(); it< list.end(); it++) {
      setTypeSize(*it);
      size += (*it)->getSize();
    }
  }
  cls->setSize(size);
}

void setEntitySizes(SymTable * symtab) {
  if (symtab==NULL) { return; }

  vector<Entity *>::size_type sz = symtab->table.size();
  switch(symtab->type) {
  case GLOBAL:
    // first set local variable sizes for class
    for(int i=0;i<sz;i++) {
      Entity *e = symtab->table[i];
      setClassSize(e);
      setEntitySizes(e->getSymTable());
    }
    break;
  case CLS:
    for(int i=0;i<sz;i++) {
      Entity *e = symtab->table[i];
      setTypeSize(e);
      setEntitySizes(e->getSymTable());
    }
    break;
  case METHOD:
    for(int i=0;i<sz;i++) {
      Entity *e = symtab->table[i];
      setEntitySizes(e->getSymTable());
    }
    break;
  case PARAMS:
    for(int i=0;i<sz;i++) {
      Entity *e = symtab->table[i];
      setTypeSize(e);
    }
    break;
  case LOCALS:
    for(int i=0;i<sz;i++) {
      Entity *e = symtab->table[i];
      setTypeSize(e);
    }
    break;
  default:
    break;
  }
}

void setEntityOffsets(SymTable * symtab) {
  if (symtab==NULL) { return ; }

  int offset = 0;
  vector<Entity *>::size_type sz = symtab->table.size();
  switch(symtab->type) {
  case GLOBAL:
    // doesn't matter
    for(int i=0;i<sz;i++){
      Entity *e = symtab->table[i];
      setEntityOffsets(e->getSymTable());
    }
    break;
  case CLS:
    for(int i=0;i<sz;i++) {
      Entity *e = symtab->table[i];
      e->setOffset(offset);
      offset += e->_size;
      setEntityOffsets(e->getSymTable());
    }
    break;
  case METHOD:
    for (int i=0;i<sz;i++) {
      Entity *e = symtab->table[i];
      setEntityOffsets(e->getSymTable());
    }
    break;
  case PARAMS:
    for(int i=0;i<sz;i++) {
      Entity *e = symtab->table[i];
      offset += e->_size;
      e->setOffset(offset);
    }
    break;
  case LOCALS:
    for(int i=0;i<sz;i++) {
      Entity *e = symtab->table[i];
      e->setOffset(offset);
      offset -= e->_size;
    }
    break;
  default:
    break;
  }
}

PTNode * getMainNode ( PTNode * node)
{
  if (node==NULL) { return NULL; }
  vector<PTNode *> * children = node->getChildren();
  vector<PTNode *>::iterator it;
  for (it = children->begin(); it < children->end(); it++){
    PTNode * nd = (*it);
    if (nd == NULL){ continue; }

    if (nd->getModel() == METHOD_DECLARATION ) {
      MethodDeclaration * md = (MethodDeclaration *)nd;
      MethodHeader * mh = (MethodHeader *) md->methodHeader();
      MethodDeclarator * mdr = (MethodDeclarator *) mh->methodDeclarator();
      LeafNode * ln = (LeafNode *) mdr->ID();
      const char * name = ln->_str;
      if( strcmp(name, "main") == 0 ){
	return nd;
      }
    } else {
      PTNode * maybe = getMainNode(nd);
      if (maybe != NULL) { return maybe; }
    }
  }

  return NULL;
}


void genCode (ScopeStack * ss, iostream & ios)
{
  SymTable * cur = ss->cur();
  
  if( cur->type == METHOD ){
    // push params and locals onto stack
    SymTable *params = cur->find("params")->getSymTable();
    SymTable *locals = cur->find("locals")->getSymTable();
    ss->push(params);
    ss->push(locals);
    // run Code.genCode
    PTNode * codeTree = cur->find("code")->getParseTree();
    codeTree->genCode(ss, ios);
    // pop locals
    ss->pop();
    // pop params
    ss->pop();
    return;
  }
  vector<Entity *>::iterator it;
  vector<Entity *> * tab = cur->getEntities();
  for (it=tab->begin(); it < tab->end(); it++) {
    Entity * e = *it;
    if( strcmp(e->getType(), "class") == 0){
      // push class onto stack
      ss->push(e->getSymTable());
      // gencode the class
      genCode(ss, ios);
      // when done, return stack to how it was
      ss->pop();
    }
    else if( strcmp(e->getType(), "meth") == 0 || e->isMethod){
      // push method onto stack
      ss->push(e->getSymTable());
      // gencode for the method
      genCode(ss, ios);
      // return stack to previous state
      ss->pop();
      ss->setCurMethLabel(e->codeName());
    }
  }
  // ss back to global state
}

void
genData(ScopeStack *ss, iostream & ios)
{
  // set data
  ios<<".data"<<endl;
  ios<<"EOL:\t.asciiz \"\\n\""<<endl;
  ios<<endl;
}

Entity *
genTemp(SymTable * st, const char * type)
{
  //generate new name
  int next = atoi(tvar+1) + 1;
  sprintf(tvar, "t%d", next);
  const char * s = strdup(tvar);
  Entity * e = new Entity(s, type);
  st->insert(e);
  return e;
}

const char *
genLabel()
{
  int next = atoi(label+1) + 1;
  sprintf(label, "l%d", next);
  return strdup(label);
}


void attachEntitiesToIds(PTNode * node)
{  
  if (node==NULL) { return ; }

  vector<PTNode *> * children = node->getChildren();
  vector<PTNode *>::iterator it;

  for (it = children->begin(); it < children->end(); it++){
    PTNode * nd = (*it);
    if (nd==NULL) { continue ; }
    attachEntitiesToIds(nd);
  }

}

bool
goesToLiteral(PTNode * node){
  if (node == NULL){ return false; }
  if (node->getModel()==LITERAL){
    return true;
  }
  vector<PTNode *> * children = node->getChildren();
  if (children->size()==1){
    return goesToLiteral(children->at(0));
  } else {
    return false;
  }
}


bool
evalToTrue(PTNode * node){
  return true;
  // TODO
}


bool
evalToFalse(PTNode * node){
  return false;
  // TODO
}
