#Alejandro Guillén Merino		DNI: 48790456G
OPTS = -Wall -Wextra -O3 -std=c++0x

all: qs-vs-hs qs-vs-hs.MSteps graphs

graphs: qs-vs-hs.MSteps qs-vs-hs.gpi
	gnuplot qs-vs-hs.gpi

qs-vs-hs: qs-vs-hs.cc
	g++ ${OPTS} -o qs-vs-hs qs-vs-hs.cc	 

qs-vs-hs.MSteps: qs-vs-hs
	./qs-vs-hs > qs-vs-hs.MSteps

tar: qs-vs-hs.cc qs-vs-hs.gpi makefile
	tar cvfz 48790456G.tar.gz qs-vs-hs.cc qs-vs-hs.gpi makefile

clean: 
	rm -f qs-vs-hs qs-vs-hs.MSteps fit.log qs-vs-hs*.png quickSort.png heapSort.png *~
