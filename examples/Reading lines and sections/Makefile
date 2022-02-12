CPPFLAGS = -std=c++11 -Wall -Werror -pedantic

LibCfgPP: main.o LibCfgPP.o
	g++ $(CPPFLAGS) main.o LibCfgPP.o && make clean

main.o: main.cpp
	g++ $(CPPFLAGS) -c main.cpp

LibCfgPP.o: ../../LibCfgPP.cpp ../../LibCfgPP.hpp
	g++ $(CPPFLAGS) -c ../../LibCfgPP.cpp

clean:
	del *.o