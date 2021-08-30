#makefile
out: main.o list.o; gcc -o out main.o list.o
main.o: main.c udt.h; gcc -c main.c
list.o: list.c udt.h; gcc -c list.c
clean: ;rm -rf *.o out
