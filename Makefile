###############Compiler###############
CC=g++

##############Flags###################
LD_FLAGS=-std=c++11
FILE_SYSTEM_FLAGS=-lstdc++fs
FLAGS=$(LD_FLAGS) $(FILE_SYSTEM_FLAGS)
INCLUDE=-I$(shell pwd)

#############Files###################
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
BIN=msh

################Tasks################
all: $(BIN)

%.o: %.cpp
	$(CC) -c $< -o $@ $(FLAGS) $(INCLUDE)

$(BIN): $(OBJ)
	$(CC) $^ -o $@  $(FLAGS) $(INCLUDE)

clean:
	rm $(OBJ) $(BIN)

.PHONY: all clean
