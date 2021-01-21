
# MAIN PROGRAM #
OBJS 	  = main.o list.o hash_table.o parsing_functions.o word_info.o vocabulary.o hash_table_vocabulary.o data_for_training.o logisticReg.o hash_table_pairs.o
SOURCE	  = .main.c ../Project2/testing.c ../Project2/list/list.c ./hash_table/hash_table.c ./reading/parsing_functions.c ../Project2/word_info/word_info.c ../Project2/vocabulary/vocabulary.c ../Project2/vocabulary/hash_table_vocabulary.c ./classification/data_for_training.c ./classification/logisticReg.c ./classification/hash_table_pairs.c
HEADER    = ../Project2/list/list.h ../Project2/list/node.h ../Project2/list/node.h ./hash_table/hash_table.h ./reading/parsing_functions.h ./hash_table/bucket.h ../Project2/word_info/word_info.h ../Project2/vocabulary/vocabulary.h ../Project2/vocabulary/hash_table_vocabulary.h ./classification/data_for_training.h ./classification/logisticReg.h ./classification/hash_table_pairs.h
OUT  	  = prog
CC	  = gcc
FLAGS     = -g -c -Wall

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@ -lm

main.o: ./main.c
	$(CC) $(FLAGS) ./main.c

testing.o: ../Project2/testing.c
	$(CC) $(FLAGS) ../Project2/testing.c

list.o: ../Project2/list/list.c
	$(CC) $(FLAGS) ../Project2/list/list.c

hash_table.o: ./hash_table/hash_table.c
	$(CC) $(FLAGS) ./hash_table/hash_table.c

parsing_functions.o: ./reading/parsing_functions.c
	$(CC) $(FLAGS) ./reading/parsing_functions.c

word_info.o: ../Project2/word_info/word_info.c
	$(CC) $(FLAGS) ../Project2/word_info/word_info.c

vocabulary.o: ../Project2/vocabulary/vocabulary.c
	$(CC) $(FLAGS) ../Project2/vocabulary/vocabulary.c

hash_table_vocabulary.o: ../Project2/vocabulary/hash_table_vocabulary.c
	$(CC) $(FLAGS) ../Project2/vocabulary/hash_table_vocabulary.c

data_for_training.o: ./classification/data_for_training.c
	$(CC) $(FLAGS) ./classification/data_for_training.c

logisticReg.o: ./classification/logisticReg.c
	$(CC) $(FLAGS) ./classification/logisticReg.c

hash_table_pairs.o: ./classification/hash_table_pairs.c
	$(CC) $(FLAGS) ./classification/hash_table_pairs.c

# 2ND EXECUTABLE #
T_OBJS 	  = testing.o list.o hash_table.o parsing_functions.o word_info.o vocabulary.o hash_table_vocabulary.o data_for_training.o logisticReg.o hash_table_pairs.o
T_OUT    = test_prog 

$(T_OUT): $(T_OBJS)
	$(CC) -g $(T_OBJS) -o $@ -lm


# UNIT TESTING #
DEF_SRC = ../Project2/list/list.c ../Project2/word_info/word_info.c  ../Project2/vocabulary/vocabulary.c ../Project2/vocabulary/hash_table_vocabulary.c ../Project2/classification/hash_table_pairs.c ../Project2/hash_table/hash_table.c
OUTS    = test_hash_table test_list test_word_info test_vocabulary test_hash_table_pairs test_tfidf_bow 

tests:	
	cc -g ../Project2/unit_testing/test_hash_table.c $(DEF_SRC) -o test_hash_table -lm
	cc -g ../Project2/unit_testing/test_list.c $(DEF_SRC) -o test_list -lm
	cc -g ../Project2/unit_testing/test_word_info.c $(DEF_SRC) -o test_word_info -lm
	cc -g ../Project2/unit_testing/test_vocabulary.c $(DEF_SRC) -o test_vocabulary -lm
	cc -g ../Project2/unit_testing/test_hash_table_pairs.c $(DEF_SRC) -o test_hash_table_pairs -lm
	cc -g ../Project2/unit_testing/test_tfidf_bow.c $(DEF_SRC) -o test_tfidf_bow -lm

clean:
	rm -f $(OBJS) $(OUT) $(T_OBJS) $(T_OUT)
	rm -f output.txt result.txt

clean_tests:
	rm -f $(OUTS)
	rm -f output.txt



