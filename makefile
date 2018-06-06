all: prog
	rm *o

prog: Constants.o Conversion.o Structures.o Einsatzplan.o
	g++ Constants.o Conversion.o Structures.o Einsatzplan.o -o prog

SwimPlan.o: Constants.o Conversion.o Structures.o
	g++ -c src/Einsatzplan.cpp

Structures.o: Constants.o Conversion.o
	g++ -c src/Structures.cpp

Conversion.o: Constants.o
	g++ -c src/Conversion.cpp

Constants.o:
	g++ -c src/Constants.cpp

clean:
	rm *o prog