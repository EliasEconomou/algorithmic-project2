
INCLUDE = ../../include
SRC = ../../src

CC = g++
CFLAGS = -std=c++11 -Wall -g -Werror -I$(INCLUDE)
LDFLAGS = -lm

OBJS = search.o $(SRC)/point_functions.o $(SRC)/hash_table.o $(SRC)/cube_table.o $(SRC)/hash_functions.o $(SRC)/algorithms.o $(SRC)/search_methods.o
EXEC = search.out

# Arguments for make run / make valgrind
# ARGS = -i inputFile -q queryFile -k 4 -L 3 -M 100 -probes 10 -o outputFile -algorithm Frechet -metric discrete -delta 1.2
ARGS = -i ../../nasdaq2017_LQ.csv -q ../../queryFile -k 4 -L 3 -M 100 -probes 10 -o ../../outputFile -algorithm Hypercube

$(EXEC): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

valgrind: $(EXEC)
	valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all ./$(EXEC) $(ARGS)
