all: glpk heur

glpk: instance.o glpk.cpp
	g++ glpk.cpp instance.o -o glpk -Wall -g -std=c++11

heur: instance.o solution.o oc.cpp
	g++ oc.cpp instance.o solution.o -o heur -Wall -g -std=c++11
	
.cpp.o:
	g++ -Wall -g -std=c++11 -c $<
	
clean:
	rm -f *.o *~ glpk heur
