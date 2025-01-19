# Makefile

SRC = ./src/main.cpp ./src/database.cpp ./src/commands.cpp
OUT = ./main

# Default target
all:
		g++ $(SRC) -o $(OUT)

# Clean target
clean:
		rm -f $(OUT)
