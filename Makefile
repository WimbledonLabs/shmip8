#CXX = g++
#SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
#SDL_INCLUDE = -I/usr/local/include

#CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE)
#LDFLAGS = $(SDL_LIB)

#EXE = shmip8

#all: $(EXE)

#$(EXE): main.o shm_emu_io.o
#	$(CXX) $< $(LDFLAGS) -o $@

#main.o: main.cpp shm_emu_io.h
#	$(CXX) $(CXXFLAGS) $< -o $@

#shm_emu_io.o: shm_emu_io.cpp
#	$(CXX) $(CXXFLAGS) $< -o $@

#clean:
#	rm *.o && rm $(EXE)
all: main.cpp shm_emu_io.cpp
	g++ main.cpp shm_emu_io.cpp -std=c++11 -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib -o shmip

test: test.cpp shm_emu_io.cpp
	g++ test.cpp shm_emu_io.cpp -std=c++11 -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib -o test
