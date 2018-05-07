#!/bin/csh

echo "*** Project 3 Fix #1 Benchmarks ***"

# number of threads:
foreach t ( 1 2 4 )
	echo "--------------"
	echo NUMT = $t
	echo ">>>>>>>>>>>>>>"
	
	# number of subdivisions:
	foreach n ( 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 )
		#echo NUMPAD = $n
		g++ -DNUMPAD=$n -DNUMT=$t proj3_fix1.cpp -o proj3_fix1 -lm -fopenmp
		./proj3_fix1
	end

end

echo "*** Done ***"
