all: test_pt

test_pt: PTNodes.o test.cc
	g++ test.cc PTNodes.o -o test_pt

PTNodes.o: PTNodes.hh
	g++ -g -c PTNodes.cc -o PTNodes.o
clean:
	rm -rf *o test_pt
