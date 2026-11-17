CC = gcc
CFLAGS = -Wall -Wextra -pedantic -I include -MMD -MP

OBJ = bin/main.o bin/grid.o bin/engine.o bin/graphics.o bin/config.o

life: $(OBJ)
	$(CC) $(OBJ) -o $@

bin/%.o: src/%.c
	mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) life bin/*.d

.PHONY: clean

-include $(OBJ:.o=.d)