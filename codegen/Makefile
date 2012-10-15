PT_DIR = ../parsetree
ST_DIR = ../symtab

all: test_codegen

clean:
	rm -rf *o test_codegen

test_codegen: Entity.o SymTable.o codegen.cc test.cc
	g++ -g Entity.o SymTable.o codegen.cc test.cc -o test_codegen

Entity.o: $(ST_DIR)/Entity.cpp
	g++ -c -g $(ST_DIR)/Entity.cpp -o Entity.o

SymTable.o: $(ST_DIR)/SymTable.cpp
	g++ -c -g $(ST_DIR)/SymTable.cpp -o SymTable.o
