
INCLUDE = ../include
SRC = ../src

CC = g++
CFLAGS = -Ofast -ffast-math -fno-trapping-math -ftree-vectorize -std=c++11 -Wall -g -Werror -I$(INCLUDE)
LDFLAGS = -lm

OBJS = testing.o $(SRC)/point_functions.o $(SRC)/cluster_methods.o $(SRC)/hash_table.o $(SRC)/cube_table.o $(SRC)/hash_functions.o $(SRC)/algorithms.o $(SRC)/search_methods.o $(SRC)/grid_table.o $(SRC)/frechet.o $(SRC)/curve.o $(SRC)/config.o $(SRC)/point.o $(SRC)/interval.o $(SRC)/simplification.o
EXEC = testing


$(EXEC): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

valgrind: $(EXEC)
	valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all ./$(EXEC) $(ARGS)
