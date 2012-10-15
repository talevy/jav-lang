#include <string.h>
#include <iostream>
#include "../symtab/ScopeStack.h"
#include "../symtab/SymTable.h"
#include "../parsetree/PTNodes.hh"
class PTNode;
class SymTable;
class ScopeStack;
class Entity;
using namespace std;


void propagateTypeDown(PTNode *, const char *);

void setTypeSize(Entity *);
void setClassSize(Entity *);
void setClassSizes(SymTable *);
void setEntitySizes(SymTable *);
void setEntityOffsets(SymTable*);

PTNode * getMainNode(PTNode *);

void genCode(ScopeStack * ss, iostream & ios);
void genData(ScopeStack * ss, iostream & ios);
const char * genLabel();

Entity * genTemp(SymTable *, const char *);

void attachEntitiesToIds(PTNode *);


bool goesToLiteral(PTNode *);
bool evalToTrue(PTNode *);
bool evalToFalse(PTNode *);
