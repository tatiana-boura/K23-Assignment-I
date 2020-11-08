
CC=gcc
CFLAGS=-Wall


reading_part: reading_part.o functions.o 
	$(CC) $(CFLAGS) -o reading_part reading_part.o functions.o 

functions.o: functions.c
	$(CC) $(CFLAGS) -c -g functions.c


.PHONY: clean

clean:
	rm -f reading_part reading_part.o functions.o 