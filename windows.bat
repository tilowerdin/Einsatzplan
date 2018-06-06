g++ -c src\Constants.cpp
g++ -c src\Conversion.cpp
g++ -c src\Structures.cpp
g++ -c src\Einsatzplan.cpp
g++ Constants.o Conversion.o Structures.o Einsatzplan.o -o prog.exe
rm *.o