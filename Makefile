all: msh

main.o: main.c
	gcc -c $< -o $@

mshell.o: mshell.c
	gcc -c $< -o $@

msh: main.o mshell.o
	gcc $^ -o $@

clean:
	rm main.o mshell.o msh
