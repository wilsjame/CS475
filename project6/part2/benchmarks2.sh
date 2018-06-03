#!/bin/csh

echo "*** Project 6 Part Two Benchmarks ***"

# Global work size (NUM_ELEMENTS, input array size).
foreach m ( 1024 2048 4096 32768 131072 524288 1048576 2097152 4194304 8388608   )
	#echo "--------------"
	#echo NUM_ELEMENTS = $m
	#echo ">>>>>>>>>>>>>>"
	
	# Local work size (LOCAL_SIZE, work-items per work group).
	foreach n ( 16 32 128 256 512 1024 )
		#echo LOCAL_SIZE = $n
		
		#g++ -DNUMPAD=$n -DNUMT=$t proj3_fix1.cpp -o proj3_fix1 -lm -fopenmp
		g++ -DNUM_ELEMENTS=$m -DLOCAL_SIZE=$n second.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -o second -lm -fopenmp
		./second
		
	end

end

echo "*** Done ***"
