CC=gcc
CFLAGS=-Wall


reading_part: reading_part.o functions.o list.o tuples.o hash_table.o
	$(CC) $(CFLAGS) -o reading_part reading_part.o functions.o list.o tuples.o hash_table.o

list.o: list.c 
	$(CC) $(CFLAGS) -c -g list.c

tuples.o: tuples.c 
	$(CC) $(CFLAGS) -c -g tuples.c	

	
functions.o: functions.c
	$(CC) $(CFLAGS) -c -g functions.c

hash_table.o: hash_table.c
	$(CC) $(CFLAGS) -c -g hash_table.c


.PHONY: clean

clean:
	rm -f reading_part reading_part.o functions.o list.o tuples.o hash_table.o