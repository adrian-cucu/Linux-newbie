
CC = gcc
RM = rm -rf

default:  test.out

test.out: test.o
	$(CC) $^ -o $@

test.o: test.c
	$(CC) -c $^

clean:
	$(RM) *.out *.o
