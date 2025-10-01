CXX := c++
FLAGS := -Wall -Wextra -pedantic
LIBS := -lglfw3 -lm -I.

build:
	$(CXX) $(FLAGS) Main.cpp $(LIBS)
