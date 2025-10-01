FLAGS := -Wall -Wextra -pedantic
LIBS := -lglfw3 -lm -I.

build:
	$(CC) $(FLAGS) src/*.c $(LIBS)

run:
	./a.out

clean:
	rm a.out
