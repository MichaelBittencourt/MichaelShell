INCLUDES=-I$(shell pwd) #-I/home/michael/src
DEBUG=-g3
OTIMIZATION=-O0
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
BIN=msh

FLAGS= $(INCLUDES) $(DEBUG) $(OTIMIZATION)

all: $(BIN)

%.o: %.c
	gcc -c $< -o $@ $(FLAGS) 

$(BIN): $(OBJ)
	gcc $^ -o $@ $(FLAGS)

clean:
	rm $(OBJ) $(BIN)
