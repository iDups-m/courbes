CXX=g++

CXXFLAGS=-std=c++17 -Wall -I/urs/local/include
LDFLAGS=-lGL -lGLEW -lGLU -lglut -lm -larmadillo

SRC= generateCurves.cpp library.cpp
OBJ= $(SRC:.cpp=.o)
EXEC=main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf main

