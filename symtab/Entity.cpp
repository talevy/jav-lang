#include "Entity.h"

Entity::Entity(const char * name, const char * type)
{
  isMethod = false;
  _size = 0;
  _type = type;
  _name = name;
  _offset = 0;
  symtab = NULL;
  ptnode = NULL;
  if (strcmp(_type, "class") == 0) {
    symtab = new SymTable(CLS);
  }
  else if (strcmp(_type, "meth") == 0) {
    symtab = new SymTable(METHOD);
    symtab->insert(new Entity("params", "params"));
    symtab->insert(new Entity("locals", "locals"));
    symtab->insert(new Entity("code", "code"));
  }
  else if (strcmp(_type, "params")==0) {
    symtab = new SymTable(PARAMS);
  }
  else if (strcmp(_type, "locals")==0) {
    symtab = new SymTable(LOCALS);
  }
  else if (strcmp(_type,"code")==0) {
    // do not forget to set parse tree hook
    ptnode = NULL;
  }
}

Entity::~Entity()
{
}

void Entity::setParseTree(PTNode * p)
{
  ptnode = p;
}

PTNode * Entity::getParseTree()
{
  return ptnode;
}

SymTable * Entity::getSymTable()
{
  return symtab;
}

void Entity::setType(const char * t)
{
  _type = t;
}

const char * Entity::getType()
{
  return _type;
}

const char * Entity::getName()
{
  return _name;
}


void Entity::setOffset(int offset)
{
  _offset = offset;
}

int Entity::getOffset()
{
  return _offset;
}


void Entity::setSize(int size)
{
  _size = size;
}

int Entity::getSize()
{
  return _size;
}

const char *
Entity :: codeName()
{
  return _codename;
}

void
Entity :: setCodeName(const char * c)
{
  _codename = c;
}
