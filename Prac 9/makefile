#Alejandro Guillén Merino		DNI: 48790456G
OPTS = -Wall -Wextra -O3 -std=c++0x

all: maze_bb

maze: maze_bb.cc
	g++ ${OPTS} -o maze_bb maze_bb.cc	 

tar: maze_bb.cc makefile
	tar cvfz 48790456G.tar.gz maze_bb.cc memoria.pdf makefile

clean: 
	rm -f maze_bb