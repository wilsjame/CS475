CXX = g++
CXXFLAGS = -lm -fopenmp

proj2: proj2.o
	$(CXX) $(CXXFLAGS) -o proj2 proj2.o

proj2.o: proj2.cpp
	$(CXX) $(CXXFLAGS) -c proj2.cpp

clean:
	$(RM) *.o

