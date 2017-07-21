
PROG = exec
CC = gcc

INCLUDE_DIR = ../include
OBJECT_DIR  = ../obj

CFLAGS  =  -I$(INCLUDE_DIR)

_OBJ = sphere.o square.o
OBJ  = $(patsubst %,$(OBJECT_DIR)/%,$(_OBJ))

default: $(PROG)

$(PROG): $(OBJ)
	$(CC) main.c $^ $(CFLAGS) -o $(PROG)

$(OBJECT_DIR)/%.o: %.c $(INCLUDE_DIR)/%.h	
	$(CC) -c $< -o $@ $(CFLAGS)

run: $(PROG) 
	./$(PROG)

.PHONY: clean
clean:
	$(RM) $(OBJ)
