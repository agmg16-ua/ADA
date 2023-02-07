OPTS = -Wall -Wextra -O3 -std=c++0x

all: bubbleSort bubbleSort.CPUtimes graphs

graphs: bubbleSort.CPUtimes bubbleSort.gpi
	gnuplot bubbleSort.gpi

bubbleSort: bubbleSort.cc
	g++ ${OPTS} -o bubbleSort bubbleSort.cc	 

bubbleSort.CPUtimes: bubbleSort
	./bubbleSort > bubbleSort.CPUtimes

tar: bubbleSort.cc bubbleSort.gpi makefile
	tar cvfz 48790456Gtar.gz bubbleSort.cc bubbleSort.gpi makefile

clean: 
	rm -f bubbleSort bubbleSort.CPUtimes fit.log bubbleSort*.png *~
 
