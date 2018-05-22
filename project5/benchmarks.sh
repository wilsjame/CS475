#!/bin/csh

echo "*** Project 5 Benchmarks ***"
make

# Array sizes:
foreach n ( 1000 2000 4000 8000 16000 32000 64000 128000 256000 512000 1024000 2048000 4096000 8192000 16384000 32000000 )
	# echo ARRAYSIZE = $n
	g++ -D ARRAYSIZE=$n -o arraymult arraymult.cpp simd.p5.o -lm -fopenmp 
	./arraymult

end

echo "*** Done ***"
