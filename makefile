OBJ = $(patsubst %.c,%.o,$(wildcard *.c))

stegno: $(OBJ)
	gcc -o $@ $^

clean:
	rm -f *.o stegno