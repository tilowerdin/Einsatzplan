g++ -c src\Constants.cpp
g++ -c src\Conversion.cpp
g++ -c src\Structures.cpp
g++ -c src\SwimPlan.cpp
g++ Constants.o Conversion.o Structures.o SwimPlan.o -o prog.exe
rm *.o