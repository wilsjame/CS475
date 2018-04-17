CXX = g++
CXXFLAGS = -lm -fopenmp

proj1: proj1.o
	$(CXX) $(CXXFLAGS) -o proj1 proj1.o

proj1.o: proj1.cpp
	$(CXX) $(CXXFLAGS) -c proj1.cpp

clean:
	$(RM) *.o

