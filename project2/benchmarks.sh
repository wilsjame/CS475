#!/bin/csh

echo "*** Project 3 Benchmarks ***"

# number of threads:
foreach t ( 1 2 4 )
	echo "---------------------------"
	echo NUMTHREADS = $t
	g++ -DNUMTHREADS=$t proj2.cpp -o proj2 -lm -fopenmp
	./proj2
end

echo "*** Done ***"
