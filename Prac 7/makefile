#Alejandro Guillén Merino		DNI: 48790456G
OPTS = -Wall -Wextra -O3 -std=c++0x

all: maze_greedy

maze: maze_greedy.cc
	g++ ${OPTS} -o maze_greedy maze_greedy.cc	 

tar: maze_greedy.cc makefile
	tar cvfz 48790456G.tar.gz maze_greedy.cc makefile

clean: 
	rm -f maze_greedy