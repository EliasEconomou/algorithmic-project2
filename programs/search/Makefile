define newline


endef

INCLUDE = ../../include
SRC = ../../src

CC = g++
CFLAGS = -std=c++11 -Wall -g -Werror -I$(INCLUDE)
LDFLAGS = -lm

OBJS = search.o $(SRC)/point_functions.o $(SRC)/hash_table.o $(SRC)/cube_table.o $(SRC)/hash_functions.o $(SRC)/algorithms.o $(SRC)/search_methods.o $(SRC)/grid_table.o 
EXEC = search.out

#----------------------------------------------------------------------------#

    # Arguments for make run / make valgrind

    # Ai) LSH

# ARGS = -i ../../nasdaq2017_LQ.csv -q ../../myqueryFile.csv -k 4 -L 3 -M 100 -probes 10 -o ../../outputFile -algorithm LSH


    # Aii) DISCRETE FRECHET

        # DATAFILE: nasdaq2017_LQ.csv
# ARGS = -i ../../nasdaq2017_LQ.csv -q ../../myqueryFile.csv -k 5 -L 2 -M 100 -probes 10 -o ../../outputFile -algorithm Frechet -metric discrete -delta 1.1

        # DATAFILE: mydataFile.csv
ARGS = -i ../../mydataFile.csv -q ../../myqueryFile.csv -k 5 -L 2 -M 100 -probes 10 -o ../../outputFile -algorithm Frechet -metric discrete -delta 1.1


    # Aiii) CONTINUOUS FRECHET

        # DATAFILE nasdaq2017_LQ.csv
# ARGS = -i ../../nasdaq2017_LQ.csv -q ../../myqueryFile.csv -k 4 -L 1 -M 100 -probes 10 -o ../../outputFile -algorithm Frechet -metric continuous -delta 0.21

        # DATAFILE mydataFile.csv
# ARGS = -i ../../mydataFile.csv -q ../../myqueryFile.csv -k 4 -L 1 -M 100 -probes 10 -o ../../outputFile -algorithm Frechet -metric continuous -delta 0.21

#----------------------------------------------------------------------------#

$(EXEC): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

valgrind: $(EXEC)
	valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all ./$(EXEC) $(ARGS)
