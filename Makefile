FLAGS := -Wall -Wextra -pedantic
LIBS := -lglfw3 -lm -I.

build:
	$(CC) $(FLAGS) src/*.c $(LIBS)

clean:
	rm a.out
