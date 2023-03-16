#Alejandro Guillén Merino		DNI: 48790456G
OPTS = -Wall -Wextra -O3 -std=c++0x

all: pow2 pow2.Steps graphs

graphs: pow2.Steps pow2.gpi
	gnuplot pow2.gpi

pow2: pow2.cc
	g++ ${OPTS} -o pow2 pow2.cc	 

pow2.Steps: pow2
	./pow2 > pow2.Steps

tar: pow2.cc pow2.gpi makefile
	tar cvfz 48790456G.tar.gz pow2.cc pow2.gpi makefile

clean: 
	rm -f pow2 pow2.Steps fit.log *.png *~
