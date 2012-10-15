CC = g++
CFLAGS = -c
all: test_symtab

test_symtab: ScopeStack.o SymTable.o Entity.o test.o
	$(CC) Entity.o SymTable.o ScopeStack.o test.o -o test_symtab

test.o: test.cpp
	$(CC) $(CFLAGS) test.cpp

ScopeStack.o: ScopeStack.cpp
	$(CC) $(CFLAGS) ScopeStack.cpp

SymTable.o: SymTable.cpp
	$(CC) $(CFLAGS) SymTable.cpp

Entity.o: Entity.cpp
	$(CC) $(CFLAGS) Entity.cpp

clean:
	rm -rf *o test_symtab
