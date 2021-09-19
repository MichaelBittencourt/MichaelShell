###############Compiler###############
CC=g++

##############Flags###################
LD_FLAGS=-std=c++11
FILE_SYSTEM_FLAGS=-lstdc++fs
DEBUG:=
OTIMIZATION=-O3
INCLUDE=-I$(shell pwd)
DEFINES:=
FLAGS=$(LD_FLAGS) $(FILE_SYSTEM_FLAGS) $(DEBUG) $(OTIMIZATION) $(INCLUDE) $(DEFINES)

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

.PHONY: all clean debug_setup debug

debug_setup:
	$(eval DEBUG=-g3)
	$(eval OTIMIZATION=-O0)
	$(eval DEFINES=-DDEBUG $(DEFINES))

debug: debug_setup all
