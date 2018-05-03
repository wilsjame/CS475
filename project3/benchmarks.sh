#!/bin/csh

echo "*** Project 3 Benchmarks ***"

# number of threads:
foreach t ( 1 2 4 )
	echo "--------------"
	echo NUMT = $t
	echo ">>>>>>>>>>>>>>"
	
	# number of subdivisions:
	foreach n ( 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 )
		#echo NUMPAD = $n
		g++ -DNUMPAD=$n -DNUMT=$t proj3.cpp -o proj3 -lm -fopenmp
		./proj3
	end

end

echo "*** Done ***"
