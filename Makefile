CC=g++
CFLAGS=-g -Wall -std=c++11
RM=rm

default: Minesweeper

Minesweeper: MinesweeperClient.o Minesweeper.o
	$(CC) $(CFLAGS) -o Minesweeper MinesweeperClient.o Minesweeper.o

Minesweeper.o: Minesweeper.cpp Minesweeper.h
	$(CC) $(CFLAGS) -c Minesweeper.cpp

MinesweeperClient.o: MinesweeperClient.cpp
	$(CC) $(CFLAGS) -c MinesweeperClient.cpp

clean: 
	$(RM) Minesweeper *.o