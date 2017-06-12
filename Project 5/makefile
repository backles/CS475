simd.p5.o:	simd.p5.h  simd.p5.cpp
		g++  -c  simd.p5.cpp -o simd.p5.o

arraymult:	arraymult.cpp simd.p5.o
		rm -f data.csv
		g++  -o arraymult -DaSize=1000 arraymult.cpp simd.p5.o  -lm  -fopenmp
		./arraymult
		g++  -o arraymult -DaSize=10000 arraymult.cpp simd.p5.o  -lm  -fopenmp
		./arraymult
		g++  -o arraymult -DaSize=100000 arraymult.cpp simd.p5.o  -lm  -fopenmp
		./arraymult
		g++  -o arraymult -DaSize=1000000 arraymult.cpp simd.p5.o  -lm  -fopenmp
		./arraymult
		g++  -o arraymult -DaSize=10000000 arraymult.cpp simd.p5.o  -lm  -fopenmp
		./arraymult
		g++  -o arraymult -DaSize=32000000 arraymult.cpp simd.p5.o  -lm  -fopenmp
		./arraymult