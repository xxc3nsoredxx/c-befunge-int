OBJS = cbi.o stack.o
CC = gcc
LFLAGS = -Wall -Wextra -std=c99 -g
CFLAGS = $(LFLAGS) -c

cbi_win : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o cbi.exe

cbi_linux : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o cbi

cbi.o : cbi.c stack.h
	$(CC) $(CFLAGS) cbi.c

stack.o : stack.c stack.h
	$(CC) $(CFLAGS) stack.c

win: cbi_win

linux : cbi_linux
