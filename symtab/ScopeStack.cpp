

#include "ScopeStack.h"

ScopeStack::ScopeStack() {
  cur_meth_label = NULL;
}

ScopeStack::~ScopeStack() {

}

void ScopeStack::push(SymTable* table) {
	stack.push_back(table);
}

SymTable* ScopeStack::cur() const {
	return stack.back();
}

void ScopeStack::pop() {
	return stack.pop_back();
}


Entity * 
ScopeStack::find(const char * name) const
{
	for (int i=stack.size()-1; i>=0; i--) {
		if (stack[i]->hasName(name)
		    && stack[i]->type != METHOD){

			return stack[i]->find(name);
		}
	}
	return NULL;
}

void
ScopeStack::setCurMethLabel(const char * label)
{
  cur_meth_label = label;
}

const char *
ScopeStack::getMethLabel() const
{
  return cur_meth_label;
}
