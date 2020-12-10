
# MAIN PROGRAM #
OBJS 	  = reading_part.o list.o hash_table.o functions.o tuples.o word_info.o
SOURCE	  = ./reading/reading_part.c ./list/list.c ./hash_table/hash_table.c ../tuples/tuples.c ./reading/functions.c ./word_info/word_info.c
HEADER    = ./list/list.h ./list/node.h ./list/node.h ./hash_table/hash_table.h ../tuples/tuples.h ./reading/functions.h ./hash_table/bucket.h ./word_info/word_info.h
OUT  	  = prog
CC	  = gcc
FLAGS     = -g -c -Wall

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@ -lm

reading_part.o: ./reading/reading_part.c
	$(CC) $(FLAGS) ./reading/reading_part.c

list.o: ./list/list.c
	$(CC) $(FLAGS) ./list/list.c

hash_table.o: ./hash_table/hash_table.c
	$(CC) $(FLAGS) ./hash_table/hash_table.c

functions.o: ./reading/functions.c
	$(CC) $(FLAGS) ./reading/functions.c

tuples.o: ../tuples/tuples.c
	$(CC) $(FLAGS) ../tuples/tuples.c

word_info.o: ./word_info/word_info.c
	$(CC) $(FLAGS) ./word_info/word_info.c

# UNIT TESTING #
DEF_SRC = ./list/list.c ./word_info/word_info.c  ../tuples/tuples.c
OUTS    = test_hash_table test_list test_word_info

tests:	
	cc -g ./unit_testing/test_hash_table.c ./hash_table/hash_table.c $(DEF_SRC) -o test_hash_table
	cc -g ./unit_testing/test_list.c $(DEF_SRC) -o test_list
	cc -g ./unit_testing/test_word_info.c $(DEF_SRC) -o test_word_info

clean:
	rm -f $(OBJS) $(OUT)
	rm -f output.txt

clean_tests:
	rm -f $(OUTS)
	rm -f output.txt



