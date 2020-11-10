OBJS 	= reading_part.o list.o hash_table.o functions.o tuples.o
SOURCE	= ./reading/reading_part.c ./list/list.c ./hash_table/hash_table.c ./tuples/tuples.c ./reading/functions.c
HEADER  = ./list/list.h ./hash_table/hash_table.h ./tuples/tuples.h ./reading/functions.h
OUT  	= prog
CC	= gcc
FLAGS   = -g -c -Wall

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

reading_part.o: ./reading/reading_part.c
	$(CC) $(FLAGS) ./reading/reading_part.c

list.o: ./list/list.c
	$(CC) $(FLAGS) ./list/list.c

hash_table.o: ./hash_table/hash_table.c
	$(CC) $(FLAGS) ./hash_table/hash_table.c

functions.o: ./reading/functions.c
	$(CC) $(FLAGS) ./reading/functions.c

tuples.o: ./tuples/tuples.c
	$(CC) $(FLAGS) ./tuples/tuples.c

clean:
	rm -f $(OBJS) $(OUT)