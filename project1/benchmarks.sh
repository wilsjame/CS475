#!/bin/csh

echo "*** Project 1 Benchmarks ***"

# number of threads:
foreach t ( 1 2 4 )
	# echo NUMT = $t
	
	# number of subdivisions:
	foreach n ( 4 32 128 256 512 1024 3072 4096 )
		# echo NUMNODES = $n
		g++ -DNUMNODES=$n -DNUMT=$t proj1.cpp -o proj1 -lm -fopenmp
		./proj1
	end

end

echo "*** Done ***"
