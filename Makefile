CC=g++

CFLAGS= -Wall -I/urs/local/include -lGL -lGLEW -lGLU -lglut  -lm 
LDFLAGS= -larmadillo

SRC=generateCurves.cpp
OBJ= $(SRC:.cpp=.o)
EXEC=main

all: $(EXEC)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
