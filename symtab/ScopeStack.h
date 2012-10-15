#ifndef SCOPESTACK_H
#define SCOPESTACK_H

#include <deque>
#include "SymTable.h"
using namespace std;
class SymTable;
class Entity;
class ScopeStack {
	private:
		deque<SymTable*> stack;
		const char * cur_meth_label;
	public:
		ScopeStack();
		~ScopeStack();
		void push(SymTable*);
		void pop();
		SymTable* cur() const;
		Entity * find(const char *) const;
		void setCurMethLabel(const char *);
		const char * getMethLabel() const;
};

#endif
