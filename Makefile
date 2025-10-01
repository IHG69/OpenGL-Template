CXX := c++
FLAGS := -Wall -Wextra -pedantic
LIBS := -lglfw3 -lm -I.

build:
	$(CXX) $(FLAGS) src/Main.cpp $(LIBS)

clean:
	rm a.out
