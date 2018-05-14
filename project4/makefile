CXX = g++
CXXFLAGS = -lm -fopenmp

all: grainville.o
	$(CXX) $(CXXFLAGS) -o grainville grainville.o

grainville.o: grainville.cpp
	$(CXX) $(CXXFLAGS) -c grainville.cpp

clean:
	$(RM) *.o

