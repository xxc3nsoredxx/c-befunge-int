OBJS = cbi.o stack.o delta.o
CC = gcc
LFLAGS = -Wall -Wextra -std=c99
CFLAGS = $(LFLAGS) -c

cbi_win : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o cbi.exe

cbi_linux : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o cbi

cbi.o : cbi.c cbi.h stack.h delta.h
	$(CC) $(CFLAGS) cbi.c

stack.o : stack.c stack.h
	$(CC) $(CFLAGS) stack.c

delta.o : delta.c delta.h
	$(CC) $(CFLAGS) delta.c

win: cbi_win

linux : cbi_linux
