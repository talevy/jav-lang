
PT_DIR = ./parsetree
ST_DIR = ./symtab
CG_DIR = ./codegen

all : analyzer

clean:
	rm *.o analyzer lex.yy.c y.tab.h y.tab.c y.output

analyzer : y.tab.c lex.yy.o Entity.o ParseTree.o CodeGen.o SymTable.o ScopeStack.o
	g++ -g ParseTree.o Entity.o SymTable.o ScopeStack.o CodeGen.o y.tab.c lex.yy.o -o analyzer

Entity.o :
	g++ -g -c $(ST_DIR)/Entity.cpp -o Entity.o

SymTable.o: 
	g++ -g -c $(ST_DIR)/SymTable.cpp -o SymTable.o

ScopeStack.o: 
	g++ -g -c $(ST_DIR)/ScopeStack.cpp -o ScopeStack.o

CodeGen.o:
	g++ -g -c $(CG_DIR)/codegen.cc -o CodeGen.o

ParseTree.o: $(PT_DIR)/PTNodes.cc
	g++ -g -c $(PT_DIR)/PTNodes.cc -o ParseTree.o

lex.yy.o: lex.yy.c
	g++ -g -c lex.yy.c -o lex.yy.o

lex.yy.c: jav.l
	lex jav.l

y.tab.c: jav.y
	yacc -v -d -t jav.y



