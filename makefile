# Makefile

SRC = ./src/main.cpp ./src/commands.cpp ./src/database/database.cpp ./src/database/database_get.cpp
OUT = ./main

# Default target
all:
		g++ $(SRC) -o $(OUT)

# Clean target
clean:
		rm -f $(OUT)
